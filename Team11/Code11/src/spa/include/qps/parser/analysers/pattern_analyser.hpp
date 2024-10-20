#pragma once

#include "qps/parser/analysers/semantic_analyser_helper.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"

#include <memory>
#include <optional>

namespace qps::detail {
template <typename T>
auto require(const untyped::UntypedSynonym& synonym, const Synonyms& declarations,
             const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping)
    -> std::optional<std::shared_ptr<T>> {
    const auto& maybe_synonym = is_synonym_declared(declarations, mapping, synonym);
    if (!maybe_synonym.has_value()) {
        return std::nullopt;
    }

    const auto& maybe_variable = std::dynamic_pointer_cast<T>(maybe_synonym.value());
    if (!maybe_variable) {
        return std::nullopt;
    }

    return maybe_variable;
}

template <typename T, std::enable_if_t<std::is_base_of_v<Synonym, T>, int> = true>
auto require_if_syn(const untyped::UntypedEntRef& ent_ref, const Synonyms& declarations,
                    const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping) -> std::optional<EntRef> {
    return std::visit(
        overloaded{[&declarations, &mapping](const untyped::UntypedSynonym& synonym) -> std::optional<EntRef> {
                       return require<T>(synonym, declarations, mapping);
                   },
                   [](const auto& x) -> std::optional<EntRef> {
                       return x;
                   }},
        ent_ref);
}
} // namespace qps::detail

namespace qps {
class PatternAssignAnalyser {

  public:
    static auto analyse(const Synonyms& declarations,
                        const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                        const untyped::UntypedPatternClause& pattern) -> std::optional<SyntacticPattern> {
        static constexpr auto NUM_ARGS = 2;
        if (pattern.num_arg != NUM_ARGS) {
            return std::nullopt;
        }
        const auto& maybe_synonym = detail::require<AssignSynonym>(pattern.synonym, declarations, mapping);
        if (!maybe_synonym.has_value()) {
            return std::nullopt;
        }
        const auto& maybe_ent_ref = detail::require_if_syn<VarSynonym>(pattern.ent_ref, declarations, mapping);
        if (!maybe_ent_ref) {
            return std::nullopt;
        }
        return PatternAssign{maybe_synonym.value(), maybe_ent_ref.value(), pattern.expression_spec};
    }
};

class PatternWhileAnalyser {

  public:
    static auto analyse(const Synonyms& declarations,
                        const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                        const untyped::UntypedPatternClause& pattern) -> std::optional<SyntacticPattern> {
        static constexpr auto NUM_ARGS = 2;
        if (pattern.num_arg != NUM_ARGS) {
            return std::nullopt;
        }
        const auto& maybe_synonym = detail::require<WhileSynonym>(pattern.synonym, declarations, mapping);
        if (!maybe_synonym.has_value()) {
            return std::nullopt;
        }
        const auto maybe_ent_ref = detail::require_if_syn<VarSynonym>(pattern.ent_ref, declarations, mapping);
        if (!maybe_ent_ref.has_value()) {
            return std::nullopt;
        }

        const auto is_wildcard = std::holds_alternative<WildCard>(pattern.expression_spec);
        if (!is_wildcard) {
            return std::nullopt;
        }

        return PatternWhile{maybe_synonym.value(), maybe_ent_ref.value()};
    }
};

class PatternIfAnalyser {

  public:
    static auto analyse(const Synonyms& declarations,
                        const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                        const untyped::UntypedPatternClause& pattern) -> std::optional<SyntacticPattern> {
        static constexpr auto NUM_ARGS = 3;
        if (pattern.num_arg != NUM_ARGS) {
            return std::nullopt;
        }
        const auto& maybe_synonym = detail::require<IfSynonym>(pattern.synonym, declarations, mapping);
        if (!maybe_synonym.has_value()) {
            return std::nullopt;
        }
        const auto maybe_ent_ref = detail::require_if_syn<VarSynonym>(pattern.ent_ref, declarations, mapping);
        if (!maybe_ent_ref.has_value()) {
            return std::nullopt;
        }

        const auto is_wildcard = std::holds_alternative<WildCard>(pattern.expression_spec);
        if (!is_wildcard) {
            return std::nullopt;
        }

        return PatternIf{maybe_synonym.value(), maybe_ent_ref.value()};
    }
};

using DefaultPatternAnalysersList = TypeList<PatternAssignAnalyser, PatternWhileAnalyser, PatternIfAnalyser>;
} // namespace qps
