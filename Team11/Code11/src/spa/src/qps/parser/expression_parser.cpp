#include "qps/parser/expression_parser.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/parser_helper.hpp"
#include "qps/tokeniser/binop.hpp"
#include <optional>
#include <tuple>

namespace qps {
using Token = tokenizer::Token;
using TokenType = tokenizer::TokenType;

auto to_string(TokenType T) -> std::string {
    switch (T) {
    case TokenType::Add:
        return "+";
    case TokenType::Sub:
        return "-";
    case TokenType::Mul:
        return "*";
    case TokenType::Div:
        return "/";
    case TokenType::Mod:
        return "%";
    default:
        return "";
    }
}

auto order_traversal(const Expression& lhs, TokenType op, const Expression& rhs) -> std::string {
    return lhs.value + rhs.value + to_string(op);
}

auto wrap_parentheses(const std::string& s) -> std::string {
    return s + " ";
}

auto parse_expression_spec(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    if (is_wildcard(*it)) {
        // Case 1 and 2: Starts with Wildcard
        const auto backup = std::make_tuple(WildCard{}, it + 1); // Backup in case we fail to parse the expression

        it = std::next(it);

        // Expect left quote
        if (it == end || !is_open_quote(*it)) {
            return backup;
        }
        it = std::next(it);

        // Expect expression
        const auto maybe_expr = expression(it, end);
        if (!maybe_expr) {
            return std::nullopt;
        }
        auto [expr, new_it] = maybe_expr.value();

        // Expect right quote
        if (new_it == end || !is_close_quote(*new_it)) {
            return std::nullopt;
        }
        it = std::next(new_it);

        // Expect wildcard
        if (it == end || !is_wildcard(*it)) {
            return std::nullopt;
        }
        return std::make_tuple(PartialMatch{expr}, it + 1);
    }
#ifndef MILESTONE1
    else if (is_open_quote(*it)) {
        // Case 3: Starts with quote
        it = std::next(it);

        // Try to parse expression
        const auto maybe_expr = expression(it, end);
        if (!maybe_expr) {
            return std::nullopt;
        }
        auto [expr, new_it] = *maybe_expr;
        it = new_it;

        // Parse the closing quote
        if (it == end || !is_close_quote(*it)) {
            return std::nullopt;
        }
        return std::make_tuple(ExactMatch{expr}, it + 1);
    }
#endif
    else {
        // Default: error
        return std::nullopt;
    }
};

auto constant(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<Token>::const_iterator>> {
    if (it != end && it->T == TokenType::Integer) {
        return std::make_tuple(Expression{wrap_parentheses(it->content)}, it + 1);
    }
    return std::nullopt;
}

auto expression(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<Token>::const_iterator>> {
    // Given the left-recursive grammar, we re-write the expression as follows:
    // <expr> ::= <factor> | <expr> <binary_op> <factor>
    // We then use the operator precedence parser to parse the expressions
    if (it == end) {
        return std::nullopt;
    }

    auto maybe_lhs = factor(it, end);
    if (!maybe_lhs) {
        return std::nullopt;
    }

    auto [lhs, new_it] = *maybe_lhs;
    return bin_op_rhs(0, lhs, new_it, end);
}

auto variable(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<Token>::const_iterator>> {
    if (it != end && it->T == TokenType::String) {
        return std::make_tuple(Expression{wrap_parentheses(it->content)}, it + 1);
    }
    return std::nullopt;
}

auto factor(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    // Case 1: <const_value>
    if (auto c = constant(it, end)) {
        return c;
    }

    // Case 2: <var_name>
    if (auto v = variable(it, end)) {
        return v;
    }

    // Case 3: ( <expr> )
    if (!is_open_bracket(*it)) {
        return std::nullopt;
    }
    it = std::next(it);

    const auto maybe_expr = expression(it, end);
    if (!maybe_expr) {
        return std::nullopt;
    }
    auto [expr, new_it] = *maybe_expr;
    if (!is_close_bracket(*new_it)) {
        return std::nullopt;
    }
    return std::make_tuple(expr, std::next(new_it));
}

auto bin_op_rhs(int min_precedence, Expression lhs, std::vector<Token>::const_iterator it,
                const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Expression, std::vector<Token>::const_iterator>> {

    while (it != end) {
        // Verify that the token is a binary operator that we expect
        const auto curr_token = *it;
        const auto curr_precedence = get_precedence(curr_token.T);

        if (curr_precedence < min_precedence) {
            return std::make_tuple(lhs, it);
        }
        it = std::next(it);
        const auto op = curr_token.T;

        // Parse primary expression
        auto maybe_rhs = factor(it, end);
        if (!maybe_rhs) {
            return std::nullopt;
        }
        auto [rhs, new_it] = *maybe_rhs;
        it = new_it;

        // Exhausted all tokens --> construct the final expression
        if (it == end) {
            return std::make_tuple(Expression{wrap_parentheses(order_traversal(lhs, op, rhs))}, it);
        }

        // Check if the next operator has a higher precedence
        const auto next_token = *it;
        const auto next_precedence = get_precedence(next_token.T);

        if (curr_precedence < next_precedence) {
            // Next operator has higher precedence, so we include the "rhs" as part of the "lhs"
            // of the next operator

            maybe_rhs = bin_op_rhs(curr_precedence + 1, rhs, it, end);
            if (!maybe_rhs) {
                return std::nullopt;
            }
            rhs = std::get<0>(*maybe_rhs);
            it = std::get<1>(*maybe_rhs);
        }

        // Success: merge the lhs and rhs into a new lhs
        lhs = Expression{wrap_parentheses(order_traversal(lhs, op, rhs))};
    }
    return std::make_tuple(lhs, it);
}
} // namespace qps