#include "qps/parser/untyped/untyped_parser_helper.hpp"

namespace qps::untyped::detail {
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
        return Integer{token.content};
    }
}

auto parse_quoted_ident(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<QuotedIdent, std::vector<Token>::const_iterator>> {
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
        return parse_quoted_ident(it, end);
    }
}

auto parse_stmt_ent_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedStmtEntRef, std::vector<Token>::const_iterator>> {
    const auto& token = *it;
    if (is_stmt_ref(token)) {
        const auto& stmt_ref = parse_stmt_ref(token);
        return std::visit(
            [it](auto&& arg) -> std::optional<std::tuple<UntypedStmtEntRef, std::vector<Token>::const_iterator>> {
                return std::make_tuple(arg, std::next(it));
            },
            stmt_ref);
    } else {
        const auto maybe_ent_ref = parse_ent_ref(it, end);
        if (maybe_ent_ref.has_value()) {
            const auto& [ent_ref, rest] = maybe_ent_ref.value();
            return std::visit(
                [it](auto&& arg) -> std::optional<std::tuple<UntypedStmtEntRef, std::vector<Token>::const_iterator>> {
                    return std::make_tuple(arg, std::next(it));
                },
                ent_ref);
        }
        return std::nullopt;
    }
}
} // namespace qps::untyped::detail
