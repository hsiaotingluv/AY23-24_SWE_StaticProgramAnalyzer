#pragma once

#include "common/ast/ast.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/cond_expr_parser.hpp"
#include "sp/parser/expr_parser.hpp"
#include "sp/parser/parser.hpp"

#include <memory>

using namespace tokenizer;

namespace sp {

/*
 * stmt: read | print | call | while | if | assign
 *
 * Since read, print, call, while and if themselves can be a variable name, therefore they share the same
 * prefix with assign
 *
 * Hence, technically this is an LL(2) Grammar based on the second token
 *
 * stmt: read | print | call | while | if | assign
 */
class StatementParser : public Parser {
    ExprParser expr_parser;
    CondExprParser cond_expr_parser;

    // assign: var_name '=' expr ';'
    auto parseStmtPrime(Iterator& token_start, const Iterator& token_end, const std::string& prev_content)
        -> std::shared_ptr<AstNode>;

    // read: 'read' var_name';'
    static std::shared_ptr<AstNode> parseReadStmt(Iterator& token_start, const Token& next_token);

    // print: 'print' var_name';'
    static std::shared_ptr<AstNode> parsePrintStmt(Iterator& token_start, const Token& next_token);

    // call: 'call' proc_name';'
    static std::shared_ptr<AstNode> parseCallStmt(Iterator& token_start, const Token& next_token);

    // if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
    std::shared_ptr<AstNode> parseIfStmt(Iterator& token_start, const Parser::Iterator& token_end,
                                         const Token& next_token);

    // while: 'while' '(' cond_expr ')' '{' stmtLst '}'
    std::shared_ptr<AstNode> parseWhileStmt(Iterator& token_start, const Parser::Iterator& token_end,
                                            const Token& next_token);

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp