#include "sp/parser/rel_expr_parser.hpp"
#include "sp/parser/ast/ast.hpp"

#include <functional>
#include <unordered_map>

namespace sp {
auto RelExprParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);

    auto expression_tree = expr_parser.parse(token_start, token_end);
    auto rest_of_tree = this->parseRelPrime(token_start, token_end);
    auto casted_node = std::dynamic_pointer_cast<ComparatorNode>(rest_of_tree);
    casted_node->left = expression_tree;
    return casted_node;
}

auto RelExprParser::parseRelPrime(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);
    auto next_token = peek_next_token(token_start);

    switch (next_token.T) {
    case TokenType::GreaterThan:
    case TokenType::GreaterThanEqual:
    case TokenType::LessThan:
    case TokenType::LessThanEqual:
    case TokenType::DoubleEqual:
    case TokenType::NotEqual: {
        get_next_token(token_start);
        auto expression_tree = expr_parser.parse(token_start, token_end);

        static auto tokenToNode =
            std::unordered_map<TokenType, std::function<std::shared_ptr<ComparatorNode>(std::shared_ptr<AstNode>)>>{
                {TokenType::GreaterThan,
                 [](auto&& right) {
                     return std::make_shared<GreaterThanNode>(nullptr, std::move(right));
                 }},
                {TokenType::GreaterThanEqual,
                 [](auto&& right) {
                     return std::make_shared<GreaterThanEqualNode>(nullptr, std::move(right));
                 }},
                {TokenType::LessThan,
                 [](auto&& right) {
                     return std::make_shared<LessThanNode>(nullptr, std::move(right));
                 }},
                {TokenType::LessThanEqual,
                 [](auto&& right) {
                     return std::make_shared<LessThanEqualNode>(nullptr, std::move(right));
                 }},
                {TokenType::DoubleEqual,
                 [](auto&& right) {
                     return std::make_shared<EqualNode>(nullptr, std::move(right));
                 }},
                {TokenType::NotEqual,
                 [](auto&& right) {
                     return std::make_shared<NotEqualNode>(nullptr, std::move(right));
                 }},
            };

        return tokenToNode[next_token.T](expression_tree);
    }
    default:
        throw ParsingError("Unexpected relational comparison token");
    }
}

} // namespace sp
