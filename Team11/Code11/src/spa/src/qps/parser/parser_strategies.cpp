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
} // namespace qps::untyped::detail

namespace qps::untyped {
auto PatternParserAssignStrategy::parse_syntactic_pattern(std::vector<Token>::const_iterator it,
                                                          const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>> {
    // syn-assign '(' entRef ',' expression-spec ')'
    static constexpr auto EXPECTED_LENGTH = 6;
    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    const auto maybe_not = detail::consume_not(it, end);
    if (maybe_not.has_value()) {
        it = maybe_not.value();
    }

    // Expects string
    if (it == end || !is_string(*it)) {
        return std::nullopt;
    }
    const auto syn_assign = UntypedSynonym{IDENT{it->content}};
    it = std::next(it);

    // Expects open bracket
    if (it == end || !is_open_bracket(*it)) {
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

    if (it == end || !is_comma(*it)) {
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
    if (it == end || !is_close_bracket(*it)) {
        return std::nullopt;
    }
    it = std::next(it);

    return std::make_tuple(UntypedPatternClause{syn_assign, ent_ref, expr_spec, maybe_not.has_value()}, it);
}

auto PatternParserWhileStrategy::parse_syntactic_pattern(std::vector<Token>::const_iterator it,
                                                         const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>> {
    // syn-while '(' entRef ',' _ ')'
    static constexpr auto EXPECTED_LENGTH = 6;
    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    const auto maybe_not = detail::consume_not(it, end);
    if (maybe_not.has_value()) {
        it = maybe_not.value();
    }

    // Expects string
    const auto maybe_syn_while = *it;
    if (!is_string(maybe_syn_while)) {
        return std::nullopt;
    }
    it = std::next(it);
    const auto syn_while = UntypedSynonym{IDENT{maybe_syn_while.content}};

    // Expects open bracket
    if (it == end || !is_open_bracket(*it)) {
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
    if (it == end || !is_comma(*it)) {
        return std::nullopt;
    }
    it = std::next(it, 1);

    // Expects wildcard
    if (it == end || !is_wildcard(*it)) {
        return std::nullopt;
    }
    it = std::next(it, 1);

    // Expects closing bracket
    if (it == end || !is_close_bracket(*it)) {
        return std::nullopt;
    }
    return std::make_tuple(UntypedPatternClause{syn_while, ent_ref, WildCard{}, maybe_not.has_value()}, std::next(it));
}

auto PatternParserIfStrategy::parse_syntactic_pattern(std::vector<Token>::const_iterator it,
                                                      const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>> {
    // syn-if '(' entRef ',' _ ',' _ ')'
    static constexpr auto EXPECTED_LENGTH = 8;
    static constexpr auto EXPECTED_NUM_ARG = 3;

    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }
    const auto maybe_not = detail::consume_not(it, end);
    if (maybe_not.has_value()) {
        it = maybe_not.value();
    }

    // Expects string
    const auto maybe_syn_if = *it;
    if (!is_string(maybe_syn_if)) {
        return std::nullopt;
    }
    it = std::next(it);
    const auto syn_if = UntypedSynonym{IDENT{maybe_syn_if.content}};

    // Expects open bracket
    if (it == end || !is_open_bracket(*it)) {
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

    // Expects the following twice
    for (int i = 0; i < 2; i++) {
        // Expects comma
        if (it == end || !is_comma(*it)) {
            return std::nullopt;
        }
        it = std::next(it, 1);

        // Expects wildcard
        if (it == end || !is_wildcard(*it)) {
            return std::nullopt;
        }
        it = std::next(it, 1);
    }

    // Expects closing bracket
    if (it == end || !is_close_bracket(*it)) {
        return std::nullopt;
    }
    return std::make_tuple(UntypedPatternClause{syn_if, ent_ref, WildCard{}, maybe_not.has_value(), EXPECTED_NUM_ARG},
                           std::next(it));
}

auto WithParserStrategy::parse_clause(std::vector<Token>::const_iterator it,
                                      const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    const auto maybe_not = detail::consume_not(it, end);
    if (maybe_not.has_value()) {
        it = maybe_not.value();
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

    return std::make_tuple(UntypedWithClause{ref1, ref2, maybe_not.has_value()}, it);
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