#include "qps/optimisers/grouping.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/attribute.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

using ClauseTuple = std::tuple<uint64_t, qps::Synonyms>;

namespace qps::details {
static auto to_clause_tuples(const std::vector<std::shared_ptr<Clause>>& clauses) -> std::vector<ClauseTuple>;

static auto build_adjacency_list(const std::vector<ClauseTuple>& clause_tuples) -> std::vector<std::vector<uint64_t>>;

static auto depth_first_search(const std::vector<std::vector<uint64_t>>& adjacency_list)
    -> std::vector<std::vector<uint64_t>>;

static auto group_queries(const Query& query, const std::vector<std::vector<uint64_t>>& clause_id_forests,
                          const std::vector<ClauseTuple>& clause_tuples) -> std::vector<Query>;
} // namespace qps::details

namespace qps {
auto GroupingOptimiser::optimise(const Query& query) const -> std::vector<Query> {
    const auto clause_tuples = details::to_clause_tuples(query.clauses);
    const auto adjacency_list = details::build_adjacency_list(clause_tuples);
    const auto clause_id_forests = details::depth_first_search(adjacency_list);

    return details::group_queries(query, clause_id_forests, clause_tuples);
}
} // namespace qps

namespace qps::details {
template <typename U, typename T,
          std::enable_if_t<is_variant_member_v<std::shared_ptr<U>, T> && std::is_base_of_v<Synonym, U>, bool> = true>
static auto has_synonym(const T& ref) -> std::optional<std::shared_ptr<Synonym>> {
    if (!std::holds_alternative<std::shared_ptr<U>>(ref)) {
        return std::nullopt;
    }

    return std::get<std::shared_ptr<U>>(ref);
}

template <typename T, std::enable_if_t<is_variant_member_v<AttrRef, T>, bool> = true>
static auto has_synonym(const T& ref) -> std::optional<std::shared_ptr<Synonym>> {
    if (!std::holds_alternative<AttrRef>(ref)) {
        return std::nullopt;
    }

    const auto& attr_ref = std::get<AttrRef>(ref);
    return attr_ref.synonym;
}

static auto to_synonyms(const std::vector<std::optional<std::shared_ptr<Synonym>>>& maybe_synonyms)
    -> std::vector<std::shared_ptr<Synonym>> {
    auto synonyms = std::vector<std::shared_ptr<Synonym>>{};
    for (const auto& maybe_synonym : maybe_synonyms) {
        if (maybe_synonym.has_value()) {
            synonyms.push_back(maybe_synonym.value());
        }
    }
    return synonyms;
}

template <typename T,
          std::enable_if_t<is_variant_member_v<T, type_list_to_variant_t<DefaultStmtStmtList>>, bool> = true>
static auto relationship_to_clause_tuple(uint64_t clause_id, const T& stmt_stmt) -> std::tuple<uint64_t, Synonyms> {
    const auto syn1 = has_synonym<StmtSynonym>(stmt_stmt.stmt1);
    const auto syn2 = has_synonym<StmtSynonym>(stmt_stmt.stmt2);
    return std::make_tuple(clause_id, to_synonyms({syn1, syn2}));
}

template <typename T, std::enable_if_t<is_variant_member_v<T, type_list_to_variant_t<DefaultStmtEntList>>, bool> = true>
static auto relationship_to_clause_tuple(uint64_t clause_id, const T& stmt_ent) -> std::tuple<uint64_t, Synonyms> {
    const auto syn1 = has_synonym<StmtSynonym>(stmt_ent.stmt);
    const auto syn2 = has_synonym<VarSynonym>(stmt_ent.ent);
    return std::make_tuple(clause_id, to_synonyms({syn1, syn2}));
}

template <typename T, std::enable_if_t<is_variant_member_v<T, type_list_to_variant_t<DefaultProcVarList>>, bool> = true>
static auto relationship_to_clause_tuple(uint64_t clause_id, const T& proc_var) -> std::tuple<uint64_t, Synonyms> {
    const auto syn1 = has_synonym<ProcSynonym>(proc_var.ent1);
    const auto syn2 = has_synonym<VarSynonym>(proc_var.ent2);
    return std::make_tuple(clause_id, to_synonyms({syn1, syn2}));
}

template <typename T,
          std::enable_if_t<is_variant_member_v<T, type_list_to_variant_t<DefaultProcProcList>>, bool> = true>
static auto relationship_to_clause_tuple(uint64_t clause_id, const T& proc_proc) -> std::tuple<uint64_t, Synonyms> {
    const auto syn1 = has_synonym<ProcSynonym>(proc_proc.procedure1);
    const auto syn2 = has_synonym<ProcSynonym>(proc_proc.procedure2);
    return std::make_tuple(clause_id, to_synonyms({syn1, syn2}));
}

static auto to_clause_tuple(uint64_t clause_id, const std::shared_ptr<Clause>& clause)
    -> std::tuple<uint64_t, Synonyms> {
    if (const auto such_that_clause = std::dynamic_pointer_cast<qps::SuchThatClause>(clause)) {
        const auto relationship = such_that_clause->rel_ref;
        return std::visit(overloaded{[clause_id](const auto& relationship) -> std::tuple<uint64_t, Synonyms> {
                              return relationship_to_clause_tuple(clause_id, relationship);
                          }},
                          relationship);
    } else if (const auto pattern_clause = std::dynamic_pointer_cast<qps::PatternClause>(clause)) {
        const auto syntactic_pattern = pattern_clause->syntactic_pattern;
        return std::visit(overloaded{[clause_id](const auto& pattern) -> std::tuple<uint64_t, Synonyms> {
                              const auto syn1 = std::make_optional(pattern.get_synonym());
                              const auto syn2 = has_synonym<Synonym>(pattern.get_ent_ref());
                              return std::make_tuple(clause_id, to_synonyms({syn1, syn2}));
                          }},
                          syntactic_pattern);
    } else if (const auto with_clause = std::dynamic_pointer_cast<qps::WithClause>(clause)) {
        const auto ref1 = has_synonym(with_clause->ref1);
        const auto ref2 = has_synonym(with_clause->ref2);
        return std::make_tuple(clause_id, to_synonyms({ref1, ref2}));
    }

    return std::make_tuple(clause_id, Synonyms{});
}

static auto to_clause_tuples(const std::vector<std::shared_ptr<Clause>>& clauses) -> std::vector<ClauseTuple> {
    auto clause_tuples = std::vector<ClauseTuple>{};
    for (uint64_t i = 0; i < clauses.size(); ++i) {
        clause_tuples.push_back(to_clause_tuple(i, clauses[i]));
    }
    return clause_tuples;
}

static auto is_reachable(const ClauseTuple& from, const ClauseTuple& to) -> bool {
    // To is reachable from From if there is a synonym in From that is also in To
    const auto& [_, from_synonyms] = from;
    const auto& [__, to_synonyms] = to;

    for (const auto& from_synonym : from_synonyms) {
        for (const auto& to_synonym : to_synonyms) {
            if (from_synonym == to_synonym) {
                return true;
            }
        }
    }
    return false;
}

static auto build_adjacency_list(const std::vector<ClauseTuple>& clause_tuples) -> std::vector<std::vector<uint64_t>> {
    if (clause_tuples.empty()) {
        return {};
    }

    auto adjacency_list = std::vector<std::vector<uint64_t>>(clause_tuples.size());
    for (uint64_t i = 0; i < clause_tuples.size() - 1; ++i) {
        for (uint64_t j = i + 1; j < clause_tuples.size(); ++j) {
            if (is_reachable(clause_tuples[i], clause_tuples[j])) {
                // Reachability is a symmetric relation
                adjacency_list[i].push_back(j);
                adjacency_list[j].push_back(i);
            }
        }
    }

    return adjacency_list;
}

static auto depth_first_search(const std::vector<std::vector<uint64_t>>& adjacency_list)
    -> std::vector<std::vector<uint64_t>> {
    auto visited = std::vector<bool>(adjacency_list.size(), false);
    auto forests = std::vector<std::vector<uint64_t>>{};

    auto stack = std::vector<uint64_t>{};
    stack.reserve(adjacency_list.size());
    for (uint64_t i = 0; i < adjacency_list.size(); ++i) {
        if (visited[i]) {
            continue;
        }

        stack.push_back(i);
        auto forest = std::vector<uint64_t>{};

        while (!stack.empty()) {
            const auto current = stack.back();
            stack.pop_back();

            if (visited[current]) {
                continue;
            }

            visited[current] = true;
            forest.push_back(current);

            for (const auto& neighbour : adjacency_list[current]) {
                if (!visited[neighbour]) {
                    stack.push_back(neighbour);
                }
            }
        }

        forests.push_back(std::move(forest));
    }

    return forests;
}

static auto get_reference_length(const std::variant<BooleanReference, std::vector<Elem>>& reference) -> uint64_t {
    if (std::holds_alternative<BooleanReference>(reference)) {
        return 0;
    }

    return std::get<std::vector<Elem>>(reference).size();
}

static auto group_queries(const Query& query, const std::vector<std::vector<uint64_t>>& clause_id_forests,
                          const std::vector<ClauseTuple>& clause_tuples) -> std::vector<Query> {
    auto grouped_queries = std::vector<Query>{};
    grouped_queries.reserve(clause_id_forests.size());

    for (const auto& clause_ids : clause_id_forests) {
        auto new_declared = std::vector<std::shared_ptr<Synonym>>{};
        auto clauses = std::vector<std::shared_ptr<Clause>>{};
        clauses.reserve(clause_ids.size());

        for (const auto& clause_id : clause_ids) {
            clauses.push_back(query.clauses[clause_id]);
            const auto& [_, synonyms] = clause_tuples[clause_id];
            for (const auto& synonym : synonyms) {
                auto is_included = false;
                for (const auto& decl_prev : new_declared) {
                    if (decl_prev == synonym) {
                        is_included = true;
                        break;
                    }
                }
                if (!is_included) {
                    new_declared.emplace_back(synonym);
                }
            }
        }

        if (!std::holds_alternative<std::vector<Elem>>(query.reference) || new_declared.empty()) {
            grouped_queries.emplace_back(BooleanReference{}, clauses);
        } else {
            // new_reference is intersection of new_declared and query.reference
            const auto& reference_elem = std::get<std::vector<Elem>>(query.reference); // Safe: we checked this above

            auto old_references = std::vector<std::shared_ptr<Synonym>>{};
            old_references.reserve(reference_elem.size());
            for (const auto& elem : reference_elem) {
                std::visit(overloaded{[&old_references](const std::shared_ptr<Synonym>& synonym) {
                                          old_references.push_back(synonym);
                                      },
                                      [&old_references](const AttrRef& attr_ref) {
                                          old_references.push_back(attr_ref.synonym);
                                      }},
                           elem);
            }

            std::sort(old_references.begin(), old_references.end());
            std::sort(new_declared.begin(), new_declared.end());

            auto new_reference = std::vector<Elem>{};
            std::set_intersection(old_references.begin(), old_references.end(), new_declared.begin(),
                                  new_declared.end(), std::back_inserter(new_reference));
            if (new_reference.empty()) {
                grouped_queries.emplace_back(BooleanReference{}, clauses);
            } else {
                grouped_queries.emplace_back(new_reference, clauses);
            }
        }
    }

    // Heuristic: priorities queries that have fewer references
    std::sort(grouped_queries.begin(), grouped_queries.end(), [](const Query& lhs, const Query& rhs) {
        return get_reference_length(lhs.reference) < get_reference_length(rhs.reference);
    });
    return grouped_queries;
}
} // namespace qps::details