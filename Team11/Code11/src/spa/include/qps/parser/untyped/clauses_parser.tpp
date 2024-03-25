#pragma once

#include "qps/parser/concepts.hpp"
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"
#include <vector>

namespace qps::untyped {
template <typename Strategy, std::enable_if_t<is_parser_strategy_v<Strategy>, int> = 0>
class ClausesParser {
  public:
    using ClauseType = typename Strategy::ClauseType;

    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<std::vector<ClauseType>, std::vector<Token>::const_iterator>> {
        if (it == end) {
            return std::nullopt;
        }

        // Expects keywords
        const auto maybe_it = detail::parse_keywords(Strategy::keywords, it, end);
        if (!maybe_it.has_value()) {
            return std::nullopt;
        }
        it = maybe_it.value();

        // Expects Clauses
        auto clauses = std::vector<ClauseType>{};
        auto first_clause = true;
        while (it != end) {
            if (!first_clause) {
                const auto maybe_and = detail::consume_and(it, end);
                if (!maybe_and.has_value()) {
                    return std::make_tuple(clauses, it);
                }
                it = maybe_and.value();
            }

            const auto maybe_success = Strategy::parse_clause(it, end);
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
};
} // namespace qps::untyped