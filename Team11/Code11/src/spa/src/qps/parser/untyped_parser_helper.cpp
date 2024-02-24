#include "qps/parser/untyped/untyped_parser_helper.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/untyped/entities/query.hpp"

#include "qps/template_utils.hpp"

namespace qps::untyped::detail {
auto parse_clause(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<UntypedClause, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

auto parse_synonym(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedSynonym, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    const auto& maybe_synonym = *it;
    if (!is_string(maybe_synonym)) {
        return std::nullopt;
    }

    return std::make_tuple(UntypedSynonym{IDENT{maybe_synonym.content}}, std::next(it));
}

auto parse_stmt_ref(const Token& token) -> UntypedStmtRef {
    if (is_string(token)) {
        return UntypedSynonym{IDENT{token.content}};
    } else if (is_wildcard(token)) {
        return WildCard{};
    } else {
        const auto integer = std::stoi(token.content);
        return Integer{static_cast<uint32_t>(integer)};
    }
}

auto parse_ent_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedEntRef, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    const auto& first_token = *it;
    if (is_string(first_token)) {
        return std::make_tuple(UntypedSynonym{IDENT{first_token.content}}, std::next(it));
    } else if (is_wildcard(first_token)) {
        return std::make_tuple(WildCard{}, std::next(it));
    } else {
        // Quoted Ident
        if (std::distance(it, end) < 3) {
            return std::nullopt;
        }
        const auto& maybe_open_quote = *it;
        const auto& maybe_ident = *std::next(it);
        const auto& maybe_close_quote = *std::next(it, 2);

        if (!is_open_quote(maybe_open_quote) || !is_string(maybe_ident) || !is_close_quote(maybe_close_quote)) {
            return std::nullopt;
        }

        return std::make_tuple(QuotedIdent{maybe_ident.content}, std::next(it, 3));
    }
}

auto parse_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedRef, std::vector<Token>::const_iterator>> {
    const auto& token = *it;
    if (is_stmt_ref(token)) {
        const auto& stmt_ref = parse_stmt_ref(token);
        return std::visit(
            [it](auto&& arg) -> std::optional<std::tuple<UntypedRef, std::vector<Token>::const_iterator>> {
                return std::make_tuple(arg, std::next(it));
            },
            stmt_ref);
    } else {
        const auto maybe_ent_ref = parse_ent_ref(it, end);
        if (maybe_ent_ref.has_value()) {
            const auto& [ent_ref, rest] = maybe_ent_ref.value();
            return std::visit(
                [it](auto&& arg) -> std::optional<std::tuple<UntypedRef, std::vector<Token>::const_iterator>> {
                    return std::make_tuple(arg, std::next(it));
                },
                ent_ref);
        }
        return std::nullopt;
    }
}

auto parse_stmt_stmt(const std::string& keyword, bool is_direct) {
    return [keyword, is_direct](std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
               -> std::optional<std::tuple<UntypedStmtStmtRel, std::vector<Token>::const_iterator>> {
        constexpr auto EXPECTED_LENGTH = 6;
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
};

auto parse_ref_ent(const std::string& keyword) {
    return [keyword](std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
               -> std::optional<std::tuple<UntypedRefEntRel, std::vector<Token>::const_iterator>> {
        constexpr auto EXPECTED_LENGTH = 6;
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
}

const auto parse_follows = parse_stmt_stmt(Follows::keyword, true);
const auto parse_follows_star = parse_stmt_stmt(FollowsT::keyword, false);
const auto parse_parent = parse_stmt_stmt(Parent::keyword, true);
const auto parse_parent_star = parse_stmt_stmt(ParentT::keyword, false);
const auto parse_uses = parse_ref_ent(UsesS::keyword);
const auto parse_modifies = parse_ref_ent(ModifiesS::keyword);

auto parse_rel_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>> {
    const auto maybe_follows = parse_follows(it, end);
    if (maybe_follows.has_value()) {
        const auto [rel, rest] = maybe_follows.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    const auto maybe_follows_star = parse_follows_star(it, end);
    if (maybe_follows_star.has_value()) {
        const auto [rel, rest] = maybe_follows_star.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    const auto maybe_parent = parse_parent(it, end);
    if (maybe_parent.has_value()) {
        const auto [rel, rest] = maybe_parent.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    const auto maybe_parent_star = parse_parent_star(it, end);
    if (maybe_parent_star.has_value()) {
        const auto [rel, rest] = maybe_parent_star.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    const auto maybe_uses = parse_uses(it, end);
    if (maybe_uses.has_value()) {
        const auto [rel, rest] = maybe_uses.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    const auto maybe_modifies = parse_modifies(it, end);
    if (maybe_modifies.has_value()) {
        const auto [rel, rest] = maybe_modifies.value();
        return std::make_tuple(UntypedRelationship{rel}, rest);
    }

    return std::nullopt;
}

auto parse_declarations_rec(Synonyms&, std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&,
                            TypeList<>) -> std::optional<std::vector<Token>::const_iterator> {
    return std::nullopt;
}

} // namespace qps::untyped::detail
