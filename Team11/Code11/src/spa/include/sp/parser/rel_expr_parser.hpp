#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/expr_parser.hpp"
#include "sp/parser/parser.hpp"

using namespace tokenizer;

namespace sp {
/*
 *
 */

/**
 * @class RelationalExpressionParser
 * @brief Parses Relational expressions (i.e. >, >=, <, <=, ==, !=) in the SIMPLE language. *
 *
 * @note rel_factor: var_name | const_value | expr
 * Note that var_name and const_value are expr too therefore we can simplify
 * rel_factor: expr
 *
 * Therefore, let R = rel_expr and E = expr
 * R: E > E | E >= E | E < E | E <= E | E == E | E != E
 * Using Left Refactoring, we get
 * R: ER'
 * R': >E | >= E | < E | <= E | == E | != E
 */
class RelExprParser : public Parser {
    ExprParser expr_parser;

    /**
     * Parses the expression prime of a Relational expression.
     * @param token_start Start of the Relational expression.
     * @param token_end End of the Relational expression.
     * @return Parsed prime of the Relational expression.
     */
    auto parseRelPrime(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode>;

  public:
    /**
     * Parses a Relational expression.
     * @param token_start Start of the Relational expression.
     * @param token_end End of the Relational expression.
     * @return Parsed Relational expression.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp