#pragma once

#include "common/tokeniser/tokenizer.hpp"

#include <optional>
#include <vector>

namespace qps {

// Define ExpressionSpec to be a fully-parenthesised expression according to the grammar
struct ExpressionSpec {
    std::string value;

    friend auto operator<<(std::ostream& os, const ExpressionSpec& expressionSpec) -> std::ostream& {
        os << expressionSpec.value;
        return os;
    }

    friend auto operator==(const ExpressionSpec& lhs, const ExpressionSpec& rhs) -> bool {
        return lhs.value == rhs.value;
    }
};

using TokenType = tokenizer::TokenType;

auto wrap_parentheses(const std::string& s) -> std::string;
auto constant(std::vector<tokenizer::Token>::const_iterator it,
              const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::const_iterator>>;
auto variable(std::vector<tokenizer::Token>::const_iterator it,
              const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::const_iterator>>;
auto factor(std::vector<tokenizer::Token>::const_iterator it, const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::const_iterator>>;
auto expression(std::vector<tokenizer::Token>::const_iterator it,
                const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::const_iterator>>;

auto bin_op_rhs(int min_precedence, ExpressionSpec lhs, std::vector<tokenizer::Token>::const_iterator it,
                const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::const_iterator>>;

auto parse_expression_spec(std::vector<tokenizer::Token>::const_iterator it,
                           const std::vector<tokenizer::Token>::const_iterator& end)
    -> std::optional<std::tuple<ExpressionSpec, std::vector<tokenizer::Token>::const_iterator>>;

} // namespace qps
