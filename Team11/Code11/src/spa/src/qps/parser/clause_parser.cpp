#include "qps/parser/untyped/clause_parser.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"
#include <vector>

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

    // Expects closing bracket
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }

    const auto maybe_close_bracket = *it;
    if (!is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }
    it = std::next(it, 1);

    return std::make_tuple(std::vector<UntypedPatternClause>{UntypedPatternClause{syn_assign, ent_ref, expr_spec}}, it);
}

} // namespace qps::untyped
