#include "common/ast/statement_ast.hpp"
#include "sp/parser/statement_keyword_const.hpp"
#include "sp/parser/statement_list_parser.hpp"
#include "sp/parser/statement_parser.hpp"

namespace sp {
// if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
auto StatementParser::parseIfStmt(Parser::Iterator& token_start, const Parser::Iterator& token_end,
                                  const Token& next_token) -> std::shared_ptr<AstNode> {
    StatementListParser statement_list_parser; // avoid circular dependency
    if (next_token.T != TokenType::LParen) {
        throw ParsingError("Expecting ( for if but found other token");
    }

    auto cond_expr_tree = cond_expr_parser.parse(token_start, token_end);
    auto rparen_token = get_next_token(token_start);
    if (rparen_token.T != TokenType::RParen) {
        throw ParsingError("Expecting ) in if but found other token");
    }

    auto then_string_token = get_next_token(token_start);
    if (then_string_token.T != TokenType::String || then_string_token.content != THEN) {
        throw ParsingError("Expecting then keyword in if but found other token");
    }

    auto lcurly_token = get_next_token(token_start);
    if (lcurly_token.T != TokenType::LCurly) {
        throw ParsingError("Expecting then { keyword in if but found other token");
    }

    auto then_stmt_list_tree = statement_list_parser.parse(token_start, token_end);

    auto rcurly_token = get_next_token(token_start);
    if (rcurly_token.T != TokenType::RCurly) {
        throw ParsingError("Expecting then } keyword in if but found other token");
    }

    auto else_string_token = get_next_token(token_start);
    if (else_string_token.T != TokenType::String || else_string_token.content != ELSE) {
        throw ParsingError("Expecting else keyword in if but found other token");
    }

    lcurly_token = get_next_token(token_start);
    if (lcurly_token.T != TokenType::LCurly) {
        throw ParsingError("Expecting else { keyword in if but found other token");
    }

    auto else_stmt_list_tree = statement_list_parser.parse(token_start, token_end);

    rcurly_token = get_next_token(token_start);
    if (rcurly_token.T != TokenType::RCurly) {
        throw ParsingError("Expecting else } keyword in if but found other token");
    }

    return std::make_shared<IfNode>(cond_expr_tree, then_stmt_list_tree, else_stmt_list_tree);
}
} // namespace sp