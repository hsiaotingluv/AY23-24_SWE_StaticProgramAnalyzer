#pragma once
#include "qps/parser/analysers/semantic_analyser_helper.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace qps ::details {
inline auto validate_pattern(const Synonyms&, const std::unordered_map<std::string, std::shared_ptr<Synonym>>&,
                             const untyped::UntypedPatternClause&, TypeList<>) -> std::optional<SyntacticPattern> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto validate_pattern(const Synonyms& declarations,
                      const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                      const untyped::UntypedPatternClause& pattern, TypeList<Head, Tails...>)
    -> std::optional<SyntacticPattern> {
    const auto& maybe_success = Head::analyse(declarations, mapping, pattern);
    if (maybe_success.has_value()) {
        return maybe_success;
    }
    return validate_pattern(declarations, mapping, pattern, TypeList<Tails...>{});
}

inline auto validate_relationship(const Synonyms&, const std::unordered_map<std::string, std::shared_ptr<Synonym>>&,
                                  const untyped::UntypedRelationship&, TypeList<>) -> std::optional<Relationship> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto validate_relationship(const Synonyms& declarations,
                           const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                           const untyped::UntypedRelationship& pattern, TypeList<Head, Tails...>)
    -> std::optional<Relationship> {
    const auto& maybe_success = Head::analyse(declarations, mapping, pattern);
    if (maybe_success.has_value()) {
        return maybe_success;
    }
    return validate_relationship(declarations, mapping, pattern, TypeList<Tails...>{});
}

template <typename RelationshipAnalysersList, typename PatternAnalysersList>
auto untyped_clause_visitor(const Synonyms& declarations,
                            const std::unordered_map<std::string, std::shared_ptr<Synonym>> mapping) {
    return overloaded{
        [&declarations,
         &mapping](const untyped::UntypedSuchThatClause& such_that) -> std::optional<std::shared_ptr<Clause>> {
            const auto& maybe_rel_ref =
                validate_relationship(declarations, mapping, such_that.rel_ref, RelationshipAnalysersList{});

            if (!maybe_rel_ref.has_value()) {
                return std::nullopt;
            }

            return std::make_shared<SuchThatClause>(maybe_rel_ref.value());
        },
        [&declarations,
         &mapping](const untyped::UntypedPatternClause& pattern) -> std::optional<std::shared_ptr<Clause>> {
            auto maybe_syntactic_pattern = validate_pattern(declarations, mapping, pattern, PatternAnalysersList{});
            if (!maybe_syntactic_pattern.has_value()) {
                return std::nullopt;
            }
            return std::make_shared<PatternClause>(maybe_syntactic_pattern.value());
        },
        [&declarations, &mapping](const untyped::UntypedWithClause& with) -> std::optional<std::shared_ptr<Clause>> {
            const auto& maybe_ref1 = validate_ref(declarations, mapping, with.ref1);
            if (!maybe_ref1.has_value()) {
                return std::nullopt;
            }

            const auto& maybe_ref2 = validate_ref(declarations, mapping, with.ref2);
            if (!maybe_ref2.has_value()) {
                return std::nullopt;
            }

            const auto maybe_valid_combi = validate_ref_combination(maybe_ref1.value(), maybe_ref2.value());
            if (!maybe_valid_combi.has_value()) {
                return std::nullopt;
            }
            const auto& [ref1, ref2] = maybe_valid_combi.value();

            return std::make_shared<WithClause>(ref1, ref2);
        }};
};

} // namespace qps::details
