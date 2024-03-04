#pragma once
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/clause.hpp"

#include <array>

namespace qps::untyped {

struct PatternParserStrategy {
    static constexpr auto keywords = std::array<std::string_view, 1>{"pattern"};

    using ClauseType = UntypedPatternClause;

    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

struct WithParserStrategy {
    static constexpr auto keywords = std::array<std::string_view, 1>{"with"};

    using ClauseType = UntypedWithClause;

    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};
} // namespace qps::untyped

#include "qps/parser/untyped/clause_parser.tpp"