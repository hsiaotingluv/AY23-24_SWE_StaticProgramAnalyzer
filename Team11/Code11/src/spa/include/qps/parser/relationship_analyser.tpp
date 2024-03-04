#pragma once

#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/semantic_analyser_helper.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"

namespace qps::details {
template <typename T>
auto to_stmt_ref(const Synonyms& declarations, const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                 const T& x) -> std::optional<StmtRef> {
    return std::visit(
        overloaded{[&declarations, &mapping](const untyped::UntypedSynonym& syn) -> std::optional<StmtRef> {
                       const auto& maybe_synonym = get_stmt_synonym(declarations, mapping, syn);
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
                       const auto& maybe_synonym = is_synonym_declared(declarations, mapping, syn);
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

inline auto validate_stmt_stmt(const std::string&, const StmtRef&, const StmtRef&, TypeList<>)
    -> std::optional<Relationship> {
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

inline auto validate_stmt_ent(const std::string&, const StmtRef&, const EntRef&, TypeList<>)
    -> std::optional<Relationship> {
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

template <typename StmtEntList>
auto validate_stmt_ent(const Synonyms& declarations,
                       const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                       const std::string& keyword, const untyped::UntypedRef& ref, const EntRef& ent_ref)
    -> std::optional<Relationship> {
    const auto& maybe_stmt_ref = to_stmt_ref(declarations, mapping, ref);
    if (!maybe_stmt_ref.has_value()) {
        return std::nullopt;
    }

    const auto& stmt_ref = maybe_stmt_ref.value();
    return validate_stmt_ent(keyword, stmt_ref, ent_ref, StmtEntList{});
}

inline auto validate_ent_ent(const std::string&, const EntRef&, const EntRef&, TypeList<>)
    -> std::optional<Relationship> {
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

template <typename EntEntList>
auto validate_ent_ent(const Synonyms& declarations,
                      const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                      const std::string& keyword, const untyped::UntypedRef& ref, const EntRef& ent_ref)
    -> std::optional<Relationship> {
    const auto& maybe_stmt_ref = to_ent_ref(declarations, mapping, ref);
    if (!maybe_stmt_ref.has_value()) {
        return std::nullopt;
    }

    const auto& stmt_ref = maybe_stmt_ref.value();
    return validate_ent_ent(keyword, stmt_ref, ent_ref, EntEntList{});
}

template <typename StmtStmtList, typename StmtEntList, typename EntEntList>
auto untyped_relationship_visitor(const Synonyms& declarations,
                                  const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping) {
    return overloaded{
        [&declarations, &mapping](const untyped::UntypedStmtStmtRel& stmt_stmt_rel) -> std::optional<Relationship> {
            const auto& keyword = std::get<0>(stmt_stmt_rel);
            const auto& maybe_stmt_ref1 = to_stmt_ref(declarations, mapping, std::get<1>(stmt_stmt_rel));
            const auto& maybe_stmt_ref2 = to_stmt_ref(declarations, mapping, std::get<2>(stmt_stmt_rel));

            if (!maybe_stmt_ref1.has_value() || !maybe_stmt_ref2.has_value()) {
                return std::nullopt;
            }
            return validate_stmt_stmt(keyword, maybe_stmt_ref1.value(), maybe_stmt_ref2.value(), StmtStmtList{});
        },
        [&declarations, &mapping](const untyped::UntypedRefEntRel& ref_ent_rel) -> std::optional<Relationship> {
            const auto& [keyword, ref, ent] = ref_ent_rel;
            const auto& maybe_ent_ref = to_ent_ref(declarations, mapping, ent);

            if (!maybe_ent_ref.has_value()) {
                return std::nullopt;
            }
            const auto& ent_ref = maybe_ent_ref.value();

            const auto& maybe_stmt_ent = validate_stmt_ent<StmtEntList>(declarations, mapping, keyword, ref, ent_ref);
            if (maybe_stmt_ent.has_value()) {
                return maybe_stmt_ent;
            }

            return validate_ent_ent<EntEntList>(declarations, mapping, keyword, ref, ent_ref);
        }};
};

} // namespace qps::details