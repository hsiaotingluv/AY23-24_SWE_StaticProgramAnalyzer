#pragma once
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/clause.hpp"

#include <array>

namespace qps::untyped {
class SuchThatClausesParser {
    static constexpr auto keywords = std::array<std::string_view, 2>{"such", "that"};

  public:
    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<UntypedSuchThatClause, std::vector<Token>::const_iterator>>;
};

class PatternClausesParser {
    static constexpr auto keywords = std::array<std::string_view, 1>{"pattern"};

  public:
    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<UntypedPatternClause, std::vector<Token>::const_iterator>>;
};
} // namespace qps::untyped