#pragma once

#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"

#include "qps/template_utils.hpp"

#include <array>
#include <optional>
#include <string_view>

// Forward declarations of helper functions
namespace qps::untyped::detail {

template <typename Head, typename... Tails>
auto parse_stmt_stmt_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                         TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>>;

template <typename Head, typename... Tails>
auto parse_stmt_ent_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                        TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>>;

template <typename Head, typename... Tails>
auto parse_ent_ent_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                       TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>>;

} // namespace qps::untyped::detail

namespace qps::untyped {
template <typename SupportedStmtStmtStrategies, typename SupportedStmtEntStrategies, typename SupportedEntEntStrategies>
struct SuchThatParserStrategy {
    static constexpr auto keywords = std::array<std::string_view, 2>{"such", "that"};

    using ClauseType = UntypedSuchThatClause;

    static auto parse_clause(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>> {
        if (it == end) {
            return std::nullopt;
        }

        const auto maybe_not = detail::consume_not(it, end);
        if (maybe_not.has_value()) {
            it = maybe_not.value();
        }

        const auto maybe_stmt_stmt_rel = detail::parse_stmt_stmt_rel(it, end, SupportedStmtStmtStrategies{});
        if (maybe_stmt_stmt_rel.has_value()) {
            const auto& [rel_ref, rest] = maybe_stmt_stmt_rel.value();
            return std::make_tuple(UntypedSuchThatClause{rel_ref, maybe_not.has_value()}, rest);
        }

        const auto maybe_stmt_ent_rel = detail::parse_stmt_ent_rel(it, end, SupportedStmtEntStrategies{});
        if (maybe_stmt_ent_rel.has_value()) {
            const auto& [rel_ref, rest] = maybe_stmt_ent_rel.value();
            return std::make_tuple(UntypedSuchThatClause{rel_ref, maybe_not.has_value()}, rest);
        }

        const auto maybe_ent_ent_rel = detail::parse_ent_ent_rel(it, end, SupportedEntEntStrategies{});
        if (maybe_ent_ent_rel.has_value()) {
            const auto& [rel_ref, rest] = maybe_ent_ent_rel.value();
            return std::make_tuple(UntypedSuchThatClause{rel_ref, maybe_not.has_value()}, rest);
        }
        return std::nullopt;
    }
};

} // namespace qps::untyped

namespace qps::untyped::detail {
template <typename T>
auto parse_stmt_stmt(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedStmtStmtRel, std::vector<Token>::const_iterator>> {
    static constexpr auto EXPECTED_LENGTH = 6;

    static constexpr auto KEYWORD_INDEX = 0;
    static constexpr auto OPEN_BRACKET_INDEX = 1;
    static constexpr auto FIRST_ARG_INDEX = 2;
    static constexpr auto COMMA_INDEX = 3;
    static constexpr auto SECOND_ARG_INDEX = 4;
    static constexpr auto CLOSE_BRACKET_INDEX = 5;

    const auto keyword = std::string{T::keyword};
    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    const auto& maybe_keyword = *std::next(it, KEYWORD_INDEX);
    const auto& maybe_open_bracket = *std::next(it, OPEN_BRACKET_INDEX);
    const auto& maybe_first_arg = *std::next(it, FIRST_ARG_INDEX);
    const auto& maybe_comma = *std::next(it, COMMA_INDEX);
    const auto& maybe_second_arg = *std::next(it, SECOND_ARG_INDEX);
    const auto& maybe_close_bracket = *std::next(it, CLOSE_BRACKET_INDEX);

    if (!is_relationship_keyword(maybe_keyword, keyword) || !is_open_bracket(maybe_open_bracket) ||
        !is_comma(maybe_comma) || !is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }

    // Arguments must be a statement synonym (string), an underscore (wildcard), or a number (integer)
    if (!is_stmt_ref(maybe_first_arg) || !is_stmt_ref(maybe_second_arg)) {
        return std::nullopt;
    }

    return std::make_tuple(
        UntypedStmtStmtRel{keyword, parse_stmt_ref(maybe_first_arg), parse_stmt_ref(maybe_second_arg)},
        std::next(it, EXPECTED_LENGTH));
};

template <typename T>
auto parse_stmt_ent(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedRefEntRel, std::vector<Token>::const_iterator>> {
    constexpr auto EXPECTED_LENGTH = 6;
    const auto keyword = std::string{T::keyword};
    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    const auto maybe_keyword = *it;
    const auto maybe_open_bracket = *std::next(it, 1);
    if (!is_relationship_keyword(maybe_keyword, keyword) || !is_open_bracket(maybe_open_bracket)) {
        return std::nullopt;
    }

    // Consume confirmed tokens
    it = std::next(it, 2);

    // Get stmt_ref
    if (it == end) {
        return std::nullopt;
    }
    const auto stmt_ref = std::visit(
        [](auto&& x) -> UntypedStmtEntRef {
            return x;
        },
        parse_stmt_ref(*it));

    // Consume according to reference
    it = std::next(it);

    // Check for comma
    const auto maybe_comma = *it;
    if (!is_comma(maybe_comma)) {
        return std::nullopt;
    }
    it = std::next(it);

    // Get entity reference
    const auto maybe_ent_ref = parse_ent_ref(it, end);
    if (!maybe_ent_ref.has_value()) {
        return std::nullopt;
    }

    // Consume according to entity reference
    const auto& [ent_ref, rest2] = maybe_ent_ref.value();
    it = rest2;

    // Check for closing bracket
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }
    const auto maybe_close_bracket = *it;
    if (!is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }
    it = std::next(it, 1);
    return std::make_tuple(UntypedRefEntRel{keyword, stmt_ref, ent_ref}, it);
};

template <typename T>
auto parse_ent_ent(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedRefEntRel, std::vector<Token>::const_iterator>> {
    constexpr auto EXPECTED_LENGTH = 6;
    const auto keyword = std::string{T::keyword};
    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    const auto maybe_keyword = *it;
    const auto maybe_open_bracket = *std::next(it, 1);
    if (!is_relationship_keyword(maybe_keyword, keyword) || !is_open_bracket(maybe_open_bracket)) {
        return std::nullopt;
    }

    // Consume confirmed tokens
    it = std::next(it, 2);

    // Get reference
    const auto maybe_ref = parse_ent_ref(it, end);
    if (!maybe_ref.has_value()) {
        return std::nullopt;
    }

    // Consume according to reference
    const auto& [arg1, rest] = maybe_ref.value();
    it = rest;
    const auto ent_ref1 = std::visit(
        [](auto&& x) -> UntypedStmtEntRef {
            return x;
        },
        arg1);

    // Check for comma
    const auto maybe_comma = *it;
    if (!is_comma(maybe_comma)) {
        return std::nullopt;
    }
    it = std::next(it);

    // Get entity reference
    const auto maybe_ent_ref = parse_ent_ref(it, end);
    if (!maybe_ent_ref.has_value()) {
        return std::nullopt;
    }

    // Consume according to entity reference
    const auto& [ent_ref, rest2] = maybe_ent_ref.value();
    it = rest2;

    // Check for closing bracket
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }
    const auto maybe_close_bracket = *it;
    if (!is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }
    it = std::next(it, 1);
    return std::make_tuple(UntypedRefEntRel{keyword, ent_ref1, ent_ref}, it);
};

inline auto parse_stmt_stmt_rel(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&,
                                TypeList<>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto parse_stmt_stmt_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                         TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    const auto maybe_rel_ref = detail::parse_stmt_stmt<Head>(it, end);
    if (maybe_rel_ref.has_value()) {
        const auto& [rel, rest] = maybe_rel_ref.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    return parse_stmt_stmt_rel(it, end, TypeList<Tails...>{});
}

inline auto parse_stmt_ent_rel(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&,
                               TypeList<>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto parse_stmt_ent_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                        TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    const auto maybe_rel_ref = detail::parse_stmt_ent<Head>(it, end);
    if (maybe_rel_ref.has_value()) {
        const auto& [rel, rest] = maybe_rel_ref.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    return parse_stmt_ent_rel(it, end, TypeList<Tails...>{});
}

inline auto parse_ent_ent_rel(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto parse_ent_ent_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                       TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    const auto maybe_rel_ref = detail::parse_ent_ent<Head>(it, end);
    if (maybe_rel_ref.has_value()) {
        const auto& [rel, rest] = maybe_rel_ref.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    return parse_ent_ent_rel(it, end, TypeList<Tails...>{});
}

} // namespace qps::untyped::detail