#include "sp/parser/cond_expr_parser.hpp"
#include "sp/parser/ast/ast.hpp"
#include <memory>

using namespace tokenizer;

namespace sp {

auto CondExprParser::parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);
    auto next_token = peek_next_token(token_start);

    switch (next_token.T) {
    case TokenType::LNot: {
        get_next_token(token_start);
        auto lparen_token = get_next_token(token_start);
        if (lparen_token.T != TokenType::LParen) {
            throw ParsingError("Expecting ( for Logical Not but found other token");
        }

        auto cond_tree = this->parse(token_start, token_end);

        auto rparen_token = get_next_token(token_start);
        if (rparen_token.T != TokenType::RParen) {
            throw ParsingError("Expecting ) for Logical Not but found other token");
        }

        return std::make_shared<LogicalNotNode>(cond_tree);
    }
    default: {
        auto backup_token_start_iterator = token_start;

        // parse R | C'
        try {
            return rel_expr_parser.parse(token_start, token_end);
        } catch (const ParsingError& pe) {
            // mutate the pass by reference parameter
            token_start = backup_token_start_iterator;
            return parseCondFirstPrime(token_start, token_end);
        }
    }
    }
}

auto CondExprParser::parseCondFirstPrime(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);
    auto next_token = get_next_token(token_start);
    if (next_token.T != TokenType::LParen) {
        throw ParsingError("Expecting ( to parse Cond Prime but found other token");
    }

    auto cond_tree = this->parse(token_start, token_end);
    auto closing_token = get_next_token(token_start);
    if (closing_token.T != TokenType::RParen) {
        throw ParsingError("Expecting ) to parse Cond Prime but found other token");
    }

    auto right_tree = this->parseCondSecondPrime(token_start, token_end);
    auto casted_node = std::static_pointer_cast<BinaryNode>(right_tree);
    casted_node->left = cond_tree;
    return casted_node;
}

auto CondExprParser::parseCondSecondPrime(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);
    auto next_token = peek_next_token(token_start);
    switch (next_token.T) {
    case TokenType::LOr:
    case TokenType::LAnd: {
        get_next_token(token_start);
        auto lparen_token = get_next_token(token_start);
        if (lparen_token.T != TokenType::LParen) {
            throw ParsingError("Expecting ( to parse Cond Second Prime but found other token");
        }

        auto cond_tree = this->parse(token_start, token_end);

        auto rparen_token = get_next_token(token_start);
        if (rparen_token.T != TokenType::RParen) {
            throw ParsingError("Expecting ) to parse Cond Second Prime but found other token");
        }

        std::shared_ptr<BinaryNode> new_binop_node;

        if (next_token.T == TokenType::LOr) {
            new_binop_node = std::make_shared<BinaryNode>(NodeType::LOr);
        } else if (next_token.T == TokenType::LAnd) {
            new_binop_node = std::make_shared<BinaryNode>(NodeType::LAnd);
        }

        new_binop_node->right = cond_tree;
        return new_binop_node;
    }
    default: {
        throw ParsingError("Expecting && or || token in Cond Second Prime but found other token");
    }
    }
}

} // namespace sp