#include "sp/parser/rel_expr_parser.hpp"
#include <unordered_map>

namespace sp {
auto RelExprParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);

    auto expression_tree = expr_parser.parse(token_start, token_end);
    auto rest_of_tree = this->parseRelPrime(token_start, token_end);
    auto casted_node = std::static_pointer_cast<BinaryNode>(rest_of_tree);
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

        std::shared_ptr<BinaryNode> new_binop_node;

        static std::unordered_map<TokenType, NodeType> tokenToNode = {
            {TokenType::GreaterThan, NodeType::Gt}, {TokenType::GreaterThanEqual, NodeType::Gte},
            {TokenType::LessThan, NodeType::Lt},    {TokenType::LessThanEqual, NodeType::Lte},
            {TokenType::DoubleEqual, NodeType::Eq}, {TokenType::NotEqual, NodeType::Neq},
        };

        new_binop_node = std::make_shared<BinaryNode>(tokenToNode.at(next_token.T));
        new_binop_node->left = nullptr;
        new_binop_node->right = expression_tree;
        return new_binop_node;
    }
    default:
        throw ParsingError("Unexpected relational comparison token");
    }
}

} // namespace sp
