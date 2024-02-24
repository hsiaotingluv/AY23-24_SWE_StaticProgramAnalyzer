#pragma once

#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"

#include "qps/template_utils.hpp"

#include <array>

// Forward declarations of helper functions
namespace qps::untyped::detail {

template <typename Head, typename... Tails>
auto parse_ref_ent_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                       TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>>;

template <typename Head, typename... Tails>
auto parse_stmt_stmt_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                         TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>>;
} // namespace qps::untyped::detail

namespace qps::untyped {
template <typename SupportedStmtStmtParsers, typename SupportedRefEntParsers>
class ConcreteSuchThatClausesParser {
    static constexpr auto keywords = std::array<std::string_view, 2>{"such", "that"};

  public:
    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<UntypedSuchThatClause, std::vector<Token>::const_iterator>> {
        constexpr auto EXPECTED_LENGTH = 3;
        if (std::distance(it, end) < EXPECTED_LENGTH) {
            return std::nullopt;
        }

        const auto maybe_it = detail::parse_keywords(keywords, it, end);
        if (!maybe_it.has_value()) {
            return std::nullopt;
        }
        it = maybe_it.value();

        const auto maybe_stmt_stmt_rel = detail::parse_stmt_stmt_rel(it, end, SupportedStmtStmtParsers{});
        if (maybe_stmt_stmt_rel.has_value()) {
            const auto& [rel_ref, rest] = maybe_stmt_stmt_rel.value();
            return std::make_tuple(rel_ref, rest);
        }

        const auto maybe_ref_ent_rel = detail::parse_ref_ent_rel(it, end, SupportedRefEntParsers{});
        if (maybe_ref_ent_rel.has_value()) {
            const auto& [rel_ref, rest] = maybe_ref_ent_rel.value();
            return std::make_tuple(rel_ref, rest);
        }

        return std::nullopt;
    }
};

} // namespace qps::untyped

namespace qps::untyped::detail {
template <typename T>
auto parse_stmt_stmt(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedStmtStmtRel, std::vector<Token>::const_iterator>> {
    constexpr auto EXPECTED_LENGTH = 6;
    const auto keyword = std::string{T::keyword};
    const auto is_direct = keyword.find("*") == std::string::npos;
    const auto offset = is_direct ? 0 : 1;
    if (std::distance(it, end) < EXPECTED_LENGTH + offset) {
        return std::nullopt;
    }

    if (!is_direct) {
        // Ancestor relationship must have a star
        const auto maybe_star = std::next(it);
        if (!is_star(*maybe_star)) {
            return std::nullopt;
        }
    }

    const auto& maybe_keyword = *it;
    const auto& maybe_open_bracket = *std::next(it, 1 + offset);
    const auto& maybe_first_arg = *std::next(it, 2 + offset);
    const auto& maybe_comma = *std::next(it, 3 + offset);
    const auto& maybe_second_arg = *std::next(it, 4 + offset);
    const auto& maybe_close_bracket = *std::next(it, 5 + offset);

    if (!is_keyword(maybe_keyword, keyword.substr(0, keyword.size() - offset)) ||
        !is_open_bracket(maybe_open_bracket) || !is_comma(maybe_comma) || !is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }

    // Arguments must be a statement synonym (string), an underscore (wildcard), or a number (integer)
    if (!is_stmt_ref(maybe_first_arg) || !is_stmt_ref(maybe_second_arg)) {
        return std::nullopt;
    }

    return std::make_tuple(
        UntypedStmtStmtRel{keyword, parse_stmt_ref(maybe_first_arg), parse_stmt_ref(maybe_second_arg)},
        std::next(it, EXPECTED_LENGTH + offset));
};

template <typename T>
auto parse_ref_ent(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedRefEntRel, std::vector<Token>::const_iterator>> {
    constexpr auto EXPECTED_LENGTH = 6;
    const auto keyword = std::string{T::keyword};
    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    const auto maybe_keyword = *it;
    const auto maybe_open_bracket = *std::next(it, 1);
    if (!is_keyword(maybe_keyword, keyword) || !is_open_bracket(maybe_open_bracket)) {
        return std::nullopt;
    }

    // Consume confirmed tokens
    it = std::next(it, 2);

    // Get reference
    const auto maybe_ref = parse_ref(it, end);
    if (!maybe_ref.has_value()) {
        return std::nullopt;
    }

    // Consume according to reference
    const auto& [arg1, rest] = maybe_ref.value();
    it = rest;

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
    return std::make_tuple(UntypedRefEntRel{keyword, arg1, ent_ref}, it);
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

inline auto parse_ref_ent_rel(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto parse_ref_ent_rel(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                       TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    const auto maybe_rel_ref = detail::parse_ref_ent<Head>(it, end);
    if (maybe_rel_ref.has_value()) {
        const auto& [rel, rest] = maybe_rel_ref.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    return parse_ref_ent_rel(it, end, TypeList<Tails...>{});
}
} // namespace qps::untyped::detail