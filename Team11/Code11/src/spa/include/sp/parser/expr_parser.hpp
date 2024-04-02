#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"

#include <memory>

namespace sp {
/**
 * @class ExpressionParser
 * @brief Parses Expressions in the SIMPLE language. *
 *
 * @note Let E = expr, T = term,
 * E: E+T | E-T | T
 * Removing left recursion, we get a new grammar
 * E: TE'
 * E': e | +TE' | -TE'
 */
class ExprParser : public Parser {

    /**
     * Parses the expression prime of an expression.
     * @param token_start Start of an expression.
     * @param token_end End of an expression.
     * @return Left and Right components of an expression.
     */
    auto parseExprPrime(Iterator& token_start, const Iterator& token_end)
        -> std::tuple<std::shared_ptr<AstNode>, std::shared_ptr<AstNode>>;

  public:
    /**
     * Parses an expression.
     * @param token_start Start of an expression.
     * @param token_end End of an expression.
     * @return Parsed expression.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp