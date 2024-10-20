#pragma once

#include "common/ast/ast.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/cond_expr_parser.hpp"
#include "sp/parser/expr_parser.hpp"

namespace sp {

/**
 * @class StatementParser
 * @brief Parses different types of statements in the SIMPLE language. *
 *
 * @note stmt: read | print | call | while | if | assign
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

    /**
     * Parses the prime of a statement.
     * @param token_start Start of the statement.
     * @param token_end End of the statement.
     * @return Parsed prime of the statement.
     * @note assign: var_name '=' expr ';' # does it belong here though?
     */
    auto parseStmtPrime(Iterator& token_start, const Iterator& token_end, const std::string& prev_content)
        -> std::shared_ptr<AstNode>;

    /**
     * Parses a Read statement.
     * @param token_start Start of the Read statement.
     * @param next_token End of the Read statement.
     * @return Parsed Read statement.
     * @note read: 'read' var_name';'
     */
    static auto parseReadStmt(Iterator& token_start, const tokenizer::Token& next_token) -> std::shared_ptr<AstNode>;

    /**
     * Parses a Print statement.
     * @param token_start Start of the Print statement.
     * @param next_token End of the Print statement.
     * @return Parsed Print statement.
     * @note print: 'print' var_name';'
     */
    static auto parsePrintStmt(Iterator& token_start, const tokenizer::Token& next_token) -> std::shared_ptr<AstNode>;

    /**
     * Parses a Call statement.
     * @param token_start Start of the Call statement.
     * @param next_token End of the Call statement.
     * @return Parsed Call statement.
     * @note call: 'call' proc_name';'
     */
    static auto parseCallStmt(Iterator& token_start, const tokenizer::Token& next_token) -> std::shared_ptr<AstNode>;

    /**
     * Parses a If statement.
     * @param token_start Start of the If statement.
     * @param token_end End of the If statement.
     * @return Parsed If statement.
     * @note if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
     */
    auto parseIfStmt(Iterator& token_start, const Parser::Iterator& token_end, const tokenizer::Token& next_token)
        -> std::shared_ptr<AstNode>;

    /**
     * Parses a While statement.
     * @param token_start Start of the While statement.
     * @param token_end End of the While statement.
     * @return Parsed While statement.
     * @note while: 'while' '(' cond_expr ')' '{' stmtLst '}'
     */
    auto parseWhileStmt(Iterator& token_start, const Parser::Iterator& token_end, const tokenizer::Token& next_token)
        -> std::shared_ptr<AstNode>;

  public:
    /**
     * Parses a statement.
     * @param token_start Start of the statement.
     * @param token_end End of the statement.
     * @return Parsed statement.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp