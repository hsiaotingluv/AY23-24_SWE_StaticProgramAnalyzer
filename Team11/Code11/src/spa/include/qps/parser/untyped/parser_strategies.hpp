#pragma once
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/clause.hpp"

#include <array>

namespace qps::untyped {

template <typename SupportedStmtStmtStrategies, typename SupportedStmtEntStrategies, typename SupportedEntEntStrategies>
struct SuchThatParserStrategy; // Forward declaration - defined in parser_strategies.tpp

template <typename SupportedPatternStrategies>
struct PatternParserStrategy; // Forward declaration - defined in parser_strategies.tpp

struct PatternParserAssignStrategy {
    using ClauseType = UntypedPatternClause;

    static auto parse_syntactic_pattern(std::vector<Token>::const_iterator it,
                                        const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

struct PatternParserIfStrategy {
    using ClauseType = UntypedPatternClause;

    static auto parse_syntactic_pattern(std::vector<Token>::const_iterator it,
                                        const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

struct PatternParserWhileStrategy {
    using ClauseType = UntypedPatternClause;

    static auto parse_syntactic_pattern(std::vector<Token>::const_iterator it,
                                        const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

struct WithParserStrategy {
    static constexpr auto keywords = std::array<std::string_view, 1>{"with"};

    using ClauseType = UntypedWithClause;

    static auto parse_clause(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

} // namespace qps::untyped

#include "qps/parser/untyped/parser_strategies.tpp"