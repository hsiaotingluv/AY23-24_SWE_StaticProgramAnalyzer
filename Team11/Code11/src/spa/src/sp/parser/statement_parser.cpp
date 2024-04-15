#include "sp/parser/statement_parser.hpp"
#include "common/ast/ast.hpp"
#include "common/ast/factor_ast.hpp"
#include "common/ast/statement_ast.hpp"
#include "sp/parser/statement_keyword_const.hpp"

using namespace tokenizer;

namespace sp {
auto StatementParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);

    auto next_token = get_next_token(token_start);
    switch (next_token.T) {
    case TokenType::String: {
        return parseStmtPrime(token_start, token_end, next_token.content);
    }
    default:
        throw ParsingError("Expected variable name or special keyword token, but other token found");
    }
}

auto StatementParser::parseStmtPrime(Parser::Iterator& token_start, const Parser::Iterator& token_end,
                                     const std::string& prev_content) -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);
    auto next_token = get_next_token(token_start);

    // assign: var_name '=' expr ';'
    if (next_token.T == TokenType::Assignment) {
        auto expr_tree = expr_parser.parse(token_start, token_end);
        auto semicolon = get_next_token(token_start);
        if (semicolon.T != TokenType::Semicolon) {
            throw ParsingError("Expecting ; in assignment but found other token");
        }

        auto new_node = std::make_shared<AssignmentNode>(std::make_shared<VarNode>(prev_content),
                                                         std::dynamic_pointer_cast<ExprNode>(expr_tree));
        return new_node;
    } else {
        if (prev_content == READ) {
            return parseReadStmt(token_start, next_token);
        } else if (prev_content == PRINT) {
            return parsePrintStmt(token_start, next_token);
        } else if (prev_content == CALL) {
            return parseCallStmt(token_start, next_token);
        } else if (prev_content == IF) {
            return parseIfStmt(token_start, token_end, next_token);
        } else if (prev_content == WHILE) {
            return parseWhileStmt(token_start, token_end, next_token);
        }
    }

    throw ParsingError("Expecting to be one of the second token in statement but found other token");
}
} // namespace sp