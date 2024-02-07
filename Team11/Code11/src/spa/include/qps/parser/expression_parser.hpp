#pragma once

#include "common/tokeniser/tokenizer.hpp"

#include <optional>
#include <vector>

namespace qps {

// Define ExpressionSpec to be a fully-parenthesised expression according to the grammar
struct ExpressionSpec {
    std::string value;
};

using TokenType = tokenizer::TokenType;

auto wrap_parentheses(std::string s) -> std::string;
auto constant(std::vector<tokenizer::Token>::iterator it, const std::vector<tokenizer::Token>::iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::iterator>>;
auto variable(std::vector<tokenizer::Token>::iterator it, const std::vector<tokenizer::Token>::iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::iterator>>;
auto factor(std::vector<tokenizer::Token>::iterator it, const std::vector<tokenizer::Token>::iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::iterator>>;
auto expression(std::vector<tokenizer::Token>::iterator it, const std::vector<tokenizer::Token>::iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::iterator>>;

auto bin_op_rhs(int min_precedence, ExpressionSpec lhs, std::vector<tokenizer::Token>::iterator it,
                const std::vector<tokenizer::Token>::iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::iterator>>;

auto parse_expression_spec(std::vector<tokenizer::Token>::iterator it,
                           const std::vector<tokenizer::Token>::iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::iterator>>;

} // namespace qps
