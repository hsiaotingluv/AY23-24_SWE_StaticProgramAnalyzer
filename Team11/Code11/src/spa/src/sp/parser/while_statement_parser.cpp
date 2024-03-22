#include "common/ast/statement_ast.hpp"
#include "sp/parser/statement_list_parser.hpp"
#include "sp/parser/statement_parser.hpp"

using namespace tokenizer;

namespace sp {

// while: 'while' '(' cond_expr ')' '{' stmtLst '}'
auto StatementParser::parseWhileStmt(Parser::Iterator& token_start, const Parser::Iterator& token_end,
                                     const Token& next_token) -> std::shared_ptr<AstNode> {
    StatementListParser statement_list_parser; // avoid circular dependency
    if (next_token.T != TokenType::LParen) {
        throw ParsingError("Expecting ( for while but found other token");
    }

    auto cond_expr_tree = cond_expr_parser.parse(token_start, token_end);
    auto rparen_token = get_next_token(token_start);
    if (rparen_token.T != TokenType::RParen) {
        throw ParsingError("Expecting ) in while but found other token");
    }

    auto lcurly_token = get_next_token(token_start);
    if (lcurly_token.T != TokenType::LCurly) {
        throw ParsingError("Expecting while { keyword in while but found other token");
    }

    auto stmt_list_tree = statement_list_parser.parse(token_start, token_end);
    auto stmt_node = std::dynamic_pointer_cast<StatementListNode>(stmt_list_tree);

    auto rcurly_token = get_next_token(token_start);
    if (rcurly_token.T != TokenType::RCurly) {
        throw ParsingError("Expecting while } keyword in while but found other token");
    }

    return std::make_shared<WhileNode>(cond_expr_tree, stmt_node);
}
} // namespace sp