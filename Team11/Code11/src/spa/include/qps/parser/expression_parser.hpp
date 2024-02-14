#pragma once

#include "common/tokeniser/tokenizer.hpp"
#include "qps/parser/entities/primitives.hpp"

#include <optional>
#include <variant>
#include <vector>

namespace qps {

// Define ExpressionSpec to be a fully-parenthesised expression according to the grammar
struct Expression {
    std::string value;

    friend auto operator<<(std::ostream& os, const Expression& expression) -> std::ostream& {
        os << expression.value;
        return os;
    }

    friend auto operator==(const Expression& lhs, const Expression& rhs) -> bool {
        return lhs.value == rhs.value;
    }
};

struct PartialMatch {
    Expression expr;

    friend auto operator<<(std::ostream& os, const PartialMatch& partial_match) -> std::ostream& {
        os << partial_match.expr;
        return os;
    }

    friend auto operator==(const PartialMatch& lhs, const PartialMatch& rhs) -> bool {
        return lhs.expr == rhs.expr;
    }
};

struct ExactMatch {
    Expression expr;

    friend auto operator<<(std::ostream& os, const ExactMatch& exact_match) -> std::ostream& {
        os << exact_match.expr;
        return os;
    }

    friend auto operator==(const ExactMatch& lhs, const ExactMatch& rhs) -> bool {
        return lhs.expr == rhs.expr;
    }
};
using ExpressionSpec = std::variant<WildCard, PartialMatch, ExactMatch>;

using TokenType = tokenizer::TokenType;

auto wrap_parentheses(const std::string& s) -> std::string;
auto constant(std::vector<tokenizer::Token>::const_iterator it,
              const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<tokenizer::Token>::const_iterator>>;
auto variable(std::vector<tokenizer::Token>::const_iterator it,
              const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<tokenizer::Token>::const_iterator>>;
auto factor(std::vector<tokenizer::Token>::const_iterator it, const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<tokenizer::Token>::const_iterator>>;
auto expression(std::vector<tokenizer::Token>::const_iterator it,
                const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<tokenizer::Token>::const_iterator>>;
auto bin_op_rhs(int min_precedence, Expression lhs, std::vector<tokenizer::Token>::const_iterator it,
                const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<tokenizer::Token>::const_iterator>>;

auto parse_expression_spec(std::vector<tokenizer::Token>::const_iterator it,
                           const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::const_iterator>>;

} // namespace qps
