#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/rel_expr_parser.hpp"

using namespace tokenizer;

namespace sp {
/**
 * @class CondExprParser
 * @brief Parses conditional expressions (i.e. !, ||, &&) in the SIMPLE language.
 *
 * @note cond_expr: rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')' '&&' '(' cond_expr ')' |
 *            '(' cond_expr ')' '||' '('cond_expr ')'
 *
 * Note that this production rule is unable to be parsed to LL(k) grammar, formal proof is omitted here but
 * the intuition of the problem is that rel_expr: E > E | ..., but E can be further produce (E) recursively
 * Similarly, cond_expr can be further expanded to (C) -> ((C)) ... and so recursively
 * Therefore, theoretically, it is impossible to determine from any amount of ( tokens
 * whether we should use rel_expr production rule or (C) and maybe the other rules too.
 *
 * Therefore, one of the easiest way to tackle this issue is to use backtracking recursive descent method
 * such that
 * C: !(C) | R | C'
 * C': (C)C''
 * C'': &&(C) | ||(C)
 *
 * Note that the construction of C' is ambiguous. In short, you can have left or right associative process trees here.
 * We chose to make this right associative because its more convenient to implement, moreover ||, && are commutative
 * Hence both choices should contain valid semantics
 */
class CondExprParser : public Parser {
    RelExprParser rel_expr_parser;

    /**
     * Parse the first prime production of a conditional expression.
     * @param token_start Start of the conditional expression.
     * @param token_end End of the conditional expression.
     * @return Parsed first prime production of the conditional expression.
     */
    auto parseCondFirstPrime(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode>;

    /**
     * Parse the second prime production of a conditional expression.
     *
     * @param token_start Start of the conditional expression.
     * @param token_end End of the conditional expression.
     * @return Parsed second prime production of the conditional expression.
     */
    auto parseCondSecondPrime(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode>;

  public:
    /**
     * Parses a conditional expression.
     * @param token_start Start of the conditional expression.
     * @param token_end End of the conditional expression.
     * @return Parsed conditional expression.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp
