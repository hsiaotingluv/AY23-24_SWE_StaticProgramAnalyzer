#pragma once

#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include <memory>
#include <vector>

using namespace tokenizer;

namespace sp {
/*
 * E: E+T | E-T | T
 * Removing left recursion, we get a new grammar
 * E: TE'
 * E': e | +TE' | -TE'
 */
class ExprParser : public Parser {
    auto parseExprPrime(Iterator& token_start, const Iterator& token_end)
        -> std::tuple<std::shared_ptr<AstNode>, std::shared_ptr<AstNode>>;

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp