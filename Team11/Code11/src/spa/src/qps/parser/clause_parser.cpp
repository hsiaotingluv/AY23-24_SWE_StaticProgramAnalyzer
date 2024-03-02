#include "qps/parser/untyped/clause_parser.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"
#include <optional>
#include <string_view>
#include <vector>

namespace qps::untyped::detail {
template <unsigned long N>
auto parse_pattern_cond(const std::array<std::string_view, N>& keywords, std::vector<Token>::const_iterator it,
                        const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedPatternClause, std::vector<Token>::const_iterator>>;
} // namespace qps::untyped::detail

namespace qps::untyped {
auto PatternClausesParser::parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<std::vector<UntypedPatternClause>, std::vector<Token>::const_iterator>> {
    // pattern_cl := pattern <assign_synonym> ( <ent_ref> , <pattern_expr> )
    constexpr auto EXPECTED_LENGTH = 7;
    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    // Expects keywords
    const auto maybe_it = detail::parse_keywords(keywords, it, end);
    if (!maybe_it.has_value()) {
        return std::nullopt;
    }
    it = maybe_it.value();

    auto clauses = std::vector<UntypedPatternClause>{};
    auto first_clause = true;
    while (it != end) {
        if (!first_clause) {
            const auto maybe_and = detail::consume_and(it, end);
            if (!maybe_and.has_value()) {
                return std::make_tuple(clauses, it);
            }
            it = maybe_and.value();
        }

        const auto maybe_success = detail::parse_pattern_cond(keywords, it, end);
        if (!maybe_success.has_value()) {
            return std::nullopt;
        }
        const auto& [clause, rest] = maybe_success.value();

        clauses.push_back(clause);
        it = rest;
        first_clause = false;
    }
    return clauses.empty() ? std::nullopt : std::make_optional(std::make_tuple(clauses, it));
}
} // namespace qps::untyped

namespace qps::untyped::detail {
auto consume_and(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::vector<Token>::const_iterator> {
    if (it == end) {
        return std::nullopt;
    }
    const auto& maybe_and = *it;
    if (!is_keyword(maybe_and, "and")) {
        return std::nullopt;
    }
    return std::next(it);
}

template <unsigned long N>
auto parse_pattern_cond(const std::array<std::string_view, N>&, std::vector<Token>::const_iterator it,
                        const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedPatternClause, std::vector<Token>::const_iterator>> {
    static constexpr auto EXPECTED_LENGTH = 6;
    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    // Expects string
    const auto maybe_syn_assign = *it;
    if (!is_string(maybe_syn_assign)) {
        return std::nullopt;
    }
    it = std::next(it);
    const auto syn_assign = UntypedSynonym{IDENT{maybe_syn_assign.content}};

    // Expects open bracket
    const auto maybe_open_bracket = *it;
    if (!is_open_bracket(maybe_open_bracket)) {
        return std::nullopt;
    }
    it = std::next(it);

    // Expects entity reference
    const auto maybe_ent_ref = detail::parse_ent_ref(it, end);
    if (!maybe_ent_ref.has_value()) {
        return std::nullopt;
    }
    const auto& [ent_ref, rest] = maybe_ent_ref.value();
    it = rest;

    // Expects comma
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }
    const auto maybe_comma = *it;
    if (!is_comma(maybe_comma)) {
        return std::nullopt;
    }
    it = std::next(it, 1);

    // Expects expression spec
    const auto maybe_expr_spec = parse_expression_spec(it, end);
    if (!maybe_expr_spec.has_value()) {
        return std::nullopt;
    }
    const auto& [expr_spec, rest2] = maybe_expr_spec.value();
    it = rest2;

    // Expects closing bracket, or comma
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }

    const auto maybe_close_bracket = *it;
    if (is_close_bracket(maybe_close_bracket)) {
        it = std::next(it, 1);
        return std::make_tuple(UntypedPatternClause{syn_assign, ent_ref, expr_spec}, it);
    } else {
        // Expects comma
        const auto maybe_comma = *it;
        if (!is_comma(maybe_comma)) {
            return std::nullopt;
        }
        it = std::next(it, 1);

        // Expects wildcard
        const auto maybe_wildcard = *it;
        if (!is_wildcard(maybe_wildcard)) {
            return std::nullopt;
        }
        it = std::next(it, 1);

        // Expects closing bracket
        const auto maybe_close_bracket = *it;
        if (!is_close_bracket(maybe_close_bracket)) {
            return std::nullopt;
        }
        it = std::next(it, 1);

        // Require expression spec to be wildcard
        const auto is_wildcard = std::holds_alternative<WildCard>(expr_spec);
        if (!is_wildcard) {
            return std::nullopt;
        }

        return std::make_tuple(UntypedPatternClause{syn_assign, ent_ref, expr_spec, 3}, it);
    }
}
} // namespace qps::untyped::detail
