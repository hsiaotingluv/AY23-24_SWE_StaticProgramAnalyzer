#pragma once

#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/ast/ast.hpp"
#include "sp/parser/constant_parser.hpp"
#include "sp/parser/expr_parser.hpp"
#include "sp/parser/name_parser.hpp"
#include "sp/parser/parser.hpp"
#include <memory>
#include <vector>

using namespace tokenizer;

namespace sp {
/*
 * rel_factor: var_name | const_value | expr
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

    auto parseRelPrime(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode>;

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp