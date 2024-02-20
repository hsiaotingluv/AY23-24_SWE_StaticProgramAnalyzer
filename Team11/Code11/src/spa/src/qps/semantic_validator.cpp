#include "qps/parser/semantic_validator.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/entities/untyped/relationship.hpp"
#include "qps/parser/entities/untyped/synonym.hpp"
#include "qps/parser/errors.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <optional>
#include <unordered_map>
#include <variant>

namespace qps::details {
// Forward declarations
auto enforce_unique_declarations(const Synonyms& declarations)
    -> std::optional<std::unordered_map<std::string, std::shared_ptr<Synonym>>>;

auto is_synonym_declared(const Synonyms& declarations,
                         const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                         const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<Synonym>>;

auto validate_clause(const Synonyms& declarations,
                     const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                     const untyped::UntypedClause& clause) -> std::optional<std::shared_ptr<Clause>>;
} // namespace qps::details

namespace qps {
auto SemanticValidator::validate(const Synonyms& declarations, const untyped::UntypedQuery& untyped_query)
    -> std::variant<Query, SemanticError> {
    // Declarations must be unique
    const auto maybe_mapping = details::enforce_unique_declarations(declarations);
    if (!maybe_mapping.has_value()) {
        return SemanticError{"Non-unique mapping"};
    }
    const auto& mapping = maybe_mapping.value();

    const auto& [references, clauses] = untyped_query;

    // Reference must be declared
    const auto maybe_reference = details::is_synonym_declared(declarations, mapping, references);
    if (!maybe_reference.has_value()) {
        return SemanticError{"Undeclared reference: " + references.get_name().get_value()};
    }

    const auto& reference = maybe_reference.value();

    // Clauses must be valid
    std::vector<std::shared_ptr<Clause>> validated_clauses;
    for (const auto& clause : clauses) {
        const auto maybe_validated_clause = details::validate_clause(declarations, mapping, clause);
        if (!maybe_validated_clause.has_value()) {
            return SemanticError{"Invalid clause"};
        }
        validated_clauses.push_back(maybe_validated_clause.value());
    }

    return Query{declarations, reference, validated_clauses};
}
} // namespace qps

namespace qps ::details {
auto enforce_unique_declarations(const Synonyms& declarations)
    -> std::optional<std::unordered_map<std::string, std::shared_ptr<Synonym>>> {
    std::unordered_map<std::string, std::shared_ptr<Synonym>> mapping;

    for (const auto& declaration : declarations) {
        mapping.insert({declaration->get_name().get_value(), declaration});
    }

    return mapping.size() == declarations.size() ? std::make_optional(mapping) : std::nullopt;
}

auto is_synonym_declared(const Synonyms&, const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                         const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<Synonym>> {

    if (mapping.find(reference.get_name().get_value()) == mapping.end()) {
        return std::nullopt;
    }
    return mapping.at(reference.get_name().get_value());
}

auto get_stmt_synonym(const Synonyms& declarations,
                      const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                      const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<StmtSynonym>> {
    const auto maybe_syn = is_synonym_declared(declarations, mapping, reference);

    if (!maybe_syn.has_value()) {
        return std::nullopt;
    }

    const auto maybe_stmt_syn = std::dynamic_pointer_cast<StmtSynonym>(maybe_syn.value());
    if (!maybe_stmt_syn) {
        return std::nullopt;
    }
    return maybe_stmt_syn;
}

template <typename T>
auto to_stmt_ref(const Synonyms& declarations, const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                 const T& x) -> std::optional<StmtRef> {
    return std::visit(
        overloaded{[&declarations, &mapping](const untyped::UntypedSynonym& syn) -> std::optional<StmtRef> {
                       const auto maybe_synonym = get_stmt_synonym(declarations, mapping, syn);
                       if (!maybe_synonym.has_value()) {
                           return std::nullopt;
                       }
                       return maybe_synonym.value();
                   },
                   [](const QuotedIdent&) -> std::optional<StmtRef> {
                       return std::nullopt;
                   },
                   [](const auto& arg) -> std::optional<StmtRef> {
                       return arg;
                   }},

        x);
}

template <typename T>
auto to_ent_ref(const Synonyms& declarations, const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                const T& x) -> std::optional<EntRef> {
    return std::visit(
        overloaded{[&declarations, &mapping](const untyped::UntypedSynonym& syn) -> std::optional<EntRef> {
                       const auto maybe_synonym = is_synonym_declared(declarations, mapping, syn);
                       if (!maybe_synonym.has_value()) {
                           return std::nullopt;
                       }
                       return maybe_synonym.value();
                   },
                   [](const Integer&) -> std::optional<EntRef> {
                       return std::nullopt;
                   },
                   [](const auto& arg) -> std::optional<EntRef> {
                       return arg;
                   }},

        x);
}

auto validate_stmt_stmt(const std::string&, const StmtRef&, const StmtRef&, TypeList<>) -> std::optional<Relationship> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto validate_stmt_stmt(const std::string& keyword, const StmtRef& stmt_ref1, const StmtRef& stmt_ref2,
                        TypeList<Head, Tails...>) -> std::optional<Relationship> {

    if (Head::keyword != keyword) {
        return validate_stmt_stmt(keyword, stmt_ref1, stmt_ref2, TypeList<Tails...>{});
    } else {
        return Head{stmt_ref1, stmt_ref2};
    }
}

auto validate_stmt_ent(const std::string&, const StmtRef&, const EntRef&, TypeList<>) -> std::optional<Relationship> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto validate_stmt_ent(const std::string& keyword, const StmtRef& stmt_ref, const EntRef& ent_ref,
                       TypeList<Head, Tails...>) -> std::optional<Relationship> {

    if (Head::keyword != keyword) {
        return validate_stmt_ent(keyword, stmt_ref, ent_ref, TypeList<Tails...>{});
    } else {
        return Head::construct(stmt_ref, ent_ref);
    }
}

auto validate_stmt_ent(const Synonyms& declarations,
                       const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                       const std::string& keyword, const untyped::UntypedRef& ref, const EntRef& ent_ref)
    -> std::optional<Relationship> {
    const auto maybe_stmt_ref = to_stmt_ref(declarations, mapping, ref);
    if (!maybe_stmt_ref.has_value()) {
        return std::nullopt;
    }

    const auto stmt_ref = maybe_stmt_ref.value();
    return validate_stmt_ent(keyword, stmt_ref, ent_ref, StmtEntList{});
}

auto validate_ent_ent(const std::string&, const EntRef&, const EntRef&, TypeList<>) -> std::optional<Relationship> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto validate_ent_ent(const std::string& keyword, const EntRef& ent_ref1, const EntRef& ent_ref2,
                      TypeList<Head, Tails...>) -> std::optional<Relationship> {

    if (Head::keyword != keyword) {
        return validate_ent_ent(keyword, ent_ref1, ent_ref2, TypeList<Tails...>{});
    } else {
        return Head::construct(ent_ref1, ent_ref2);
    }
}

auto validate_ent_ent(const Synonyms& declarations,
                      const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                      const std::string& keyword, const untyped::UntypedRef& ref, const EntRef& ent_ref)
    -> std::optional<Relationship> {
    const auto maybe_stmt_ref = to_ent_ref(declarations, mapping, ref);
    if (!maybe_stmt_ref.has_value()) {
        return std::nullopt;
    }

    const auto stmt_ref = maybe_stmt_ref.value();
    return validate_ent_ent(keyword, stmt_ref, ent_ref, EntEntList{});
}

auto untyped_relationship_visitor(const Synonyms& declarations,
                                  const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping) {
    return overloaded{
        [&declarations, &mapping](const untyped::UntypedStmtStmtRel& stmt_stmt_rel) -> std::optional<Relationship> {
            const auto keyword = std::get<0>(stmt_stmt_rel);
            const auto maybe_stmt_ref1 = to_stmt_ref(declarations, mapping, std::get<1>(stmt_stmt_rel));
            const auto maybe_stmt_ref2 = to_stmt_ref(declarations, mapping, std::get<2>(stmt_stmt_rel));

            if (!maybe_stmt_ref1.has_value() || !maybe_stmt_ref2.has_value()) {
                return std::nullopt;
            }
            return validate_stmt_stmt(keyword, maybe_stmt_ref1.value(), maybe_stmt_ref2.value(), StmtStmtList{});
        },
        [&declarations, &mapping](const untyped::UntypedRefEntRel& ref_ent_rel) -> std::optional<Relationship> {
            const auto& [keyword, ref, ent] = ref_ent_rel;
            const auto maybe_ent_ref = to_ent_ref(declarations, mapping, ent);

            if (!maybe_ent_ref.has_value()) {
                return std::nullopt;
            }
            const auto ent_ref = maybe_ent_ref.value();

            const auto maybe_stmt_ent = validate_stmt_ent(declarations, mapping, keyword, ref, ent_ref);
            if (maybe_stmt_ent.has_value()) {
                return maybe_stmt_ent;
            }

            return validate_ent_ent(declarations, mapping, keyword, ref, ent_ref);
        }};
};

auto untyped_clause_visitor(const Synonyms& declarations,
                            const std::unordered_map<std::string, std::shared_ptr<Synonym>> mapping) {
    return overloaded{
        [&declarations,
         &mapping](const untyped::UntypedSuchThatClause& such_that) -> std::optional<std::shared_ptr<Clause>> {
            auto maybe_rel_ref = std::visit(untyped_relationship_visitor(declarations, mapping), such_that.rel_ref);

            if (!maybe_rel_ref.has_value()) {
                return std::nullopt;
            }

            return std::make_shared<SuchThatClause>(maybe_rel_ref.value());
        },
        [&declarations,
         &mapping](const untyped::UntypedPatternClause& pattern) -> std::optional<std::shared_ptr<Clause>> {
            const auto maybe_syn = is_synonym_declared(declarations, mapping, pattern.assign_synonym);

            if (!maybe_syn.has_value()) {
                return std::nullopt;
            }
            const auto& maybe_assign_syn = std::dynamic_pointer_cast<AssignSynonym>(maybe_syn.value());
            if (!maybe_assign_syn) {
                return std::nullopt;
            }
            const auto assign_syn = maybe_assign_syn;

            const auto maybe_ent_ref = std::visit(
                overloaded{[&declarations, &mapping](const untyped::UntypedSynonym& synonym) -> std::optional<EntRef> {
                               const auto& maybe_synonym = is_synonym_declared(declarations, mapping, synonym);
                               if (!maybe_synonym.has_value()) {
                                   return std::nullopt;
                               }

                               const auto& maybe_variable =
                                   std::dynamic_pointer_cast<VarSynonym>(maybe_synonym.value());
                               if (!maybe_variable) {
                                   return std::nullopt;
                               }
                               return maybe_variable;
                           },
                           [](const auto& x) -> std::optional<EntRef> {
                               return x;
                           }},
                pattern.ent_ref);

            if (!maybe_ent_ref.has_value()) {
                return std::nullopt;
            }

            const auto& ent_ref = maybe_ent_ref.value();
            return std::make_shared<PatternClause>(assign_syn, ent_ref, pattern.expression_spec);
        }};
};

auto validate_clause(const Synonyms& declarations,
                     const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                     const untyped::UntypedClause& clause) -> std::optional<std::shared_ptr<Clause>> {

    return std::visit(untyped_clause_visitor(declarations, mapping), clause);
}
} // namespace qps::details
