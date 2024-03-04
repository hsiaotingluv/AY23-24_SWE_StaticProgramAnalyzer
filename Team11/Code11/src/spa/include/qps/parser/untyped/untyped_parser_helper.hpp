#pragma once

#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"

namespace qps::untyped::detail {
auto parse_synonym(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedSynonym, std::vector<Token>::const_iterator>>;

auto parse_quoted_ident(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<QuotedIdent, std::vector<Token>::const_iterator>>;

auto parse_ent_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedEntRef, std::vector<Token>::const_iterator>>;

auto parse_stmt_ref(const Token& token) -> UntypedStmtRef;

auto parse_stmt_ent_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedStmtEntRef, std::vector<Token>::const_iterator>>;

auto consume_and(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::vector<Token>::const_iterator>;

template <unsigned long N>
auto parse_keywords(std::array<std::string_view, N> keywords, std::vector<Token>::const_iterator it,
                    const std::vector<Token>::const_iterator& end)
    -> std::optional<std::vector<Token>::const_iterator> {
    if (N == 0) {
        // Always successful in consuming nothing
        return it;
    }

    if (it == end) {
        // Cannot consume anything if there are no tokens
        return std::nullopt;
    }

    for (const auto& keyword : keywords) {
        if (it == end || !is_keyword(*it, std::string{keyword})) {
            return std::nullopt;
        }
        it = std::next(it);
    }

    return it;
}
} // namespace qps::untyped::detail