#include "qps/parser/untyped/parser_strategies.hpp"
#include "common/tokeniser/token_types.hpp"
#include "qps/parser/entities/attribute_name.hpp"
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/attribute.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"
#include <iterator>
#include <optional>
#include <vector>

namespace qps::untyped::detail {
auto parse_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedRef, std::vector<Token>::const_iterator>>;
}

namespace qps::untyped {
auto PatternParserStrategy::parse_clause(std::vector<Token>::const_iterator it,
                                         const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>> {
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

auto WithParserStrategy::parse_clause(std::vector<Token>::const_iterator it,
                                      const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    // Expects Ref
    const auto maybe_ref1 = detail::parse_ref(it, end);
    if (!maybe_ref1.has_value()) {
        return std::nullopt;
    }
    const auto& [ref1, rest] = maybe_ref1.value();
    it = rest;

    // Expects =
    if (it == end || !is_char<'='>(*it)) {
        return std::nullopt;
    }
    it = std::next(it);

    // Expects Ref
    const auto maybe_ref2 = detail::parse_ref(it, end);
    if (!maybe_ref2.has_value()) {
        return std::nullopt;
    }
    const auto& [ref2, rest2] = maybe_ref2.value();
    it = rest2;

    return std::make_tuple(UntypedWithClause{ref1, ref2}, it);
}
} // namespace qps::untyped

namespace qps::untyped::detail {
inline auto parse_attr_name(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<AttrName, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto parse_attr_name(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                     TypeList<Head, Tails...>)
    -> std::optional<std::tuple<AttrName, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }
    const auto maybe_keyword = *it;
    if (maybe_keyword.T != tokenizer::TokenType::AttrName || maybe_keyword.content != Head::keyword) {
        return parse_attr_name(it, end, TypeList<Tails...>{});
    }
    it = std::next(it);
    return std::make_tuple(Head{}, it);
}

auto parse_attr_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedAttrRef, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    // attrRef := <synonym> . <attr_name>

    // Expects Synonym
    const auto maybe_synonym = parse_synonym(it, end);
    if (!maybe_synonym.has_value()) {
        return std::nullopt;
    }
    const auto& [synonym, rest] = maybe_synonym.value();
    it = rest;

    // Expects .
    if (it == end || !is_char<'.'>(*it)) {
        return std::nullopt;
    }
    it = std::next(it);

    // Expects AttrName
    const auto maybe_attr_name = parse_attr_name(it, end, AttrNameList{});
    if (!maybe_attr_name.has_value()) {
        return std::nullopt;
    }
    const auto& [attr_name, rest2] = maybe_attr_name.value();
    it = rest2;

    return std::make_tuple(UntypedAttrRef{synonym, attr_name}, it);
}

auto parse_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedRef, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    // Integer
    if (is_integer(*it)) {
        return std::make_tuple(UntypedRef{Integer{it->content}}, std::next(it));
    }

    // Quoted Ident
    const auto maybe_quoted_ident = parse_quoted_ident(it, end);
    if (maybe_quoted_ident.has_value()) {
        const auto& [quoted_ident, rest] = maybe_quoted_ident.value();
        return std::make_tuple(UntypedRef{quoted_ident}, rest);
    }

    // attrRef
    const auto maybe_attr_ref = parse_attr_ref(it, end);
    if (maybe_attr_ref.has_value()) {
        const auto& [attr_ref, rest] = maybe_attr_ref.value();
        return std::make_tuple(UntypedRef{attr_ref}, rest);
    }

    return std::nullopt;
}

} // namespace qps::untyped::detail