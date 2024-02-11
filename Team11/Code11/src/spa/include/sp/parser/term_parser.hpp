#pragma once

#include "sp/parser/ast/ast.hpp"
#include "sp/parser/factor_parser.hpp"
#include "sp/parser/parser.hpp"

#include <memory>

using namespace tokenizer;

namespace sp {

/*
 * T: T*F | T/F | T%F | F
 * Removing left recursion, we get a new grammar
 * T: FT'
 * T': e | *fT' | /fT' | %fT'
 */
class TermParser : public Parser {
    FactorParser factor_parser;

    auto parseTermPrime(Iterator& token_start, const Iterator& token_end)
        -> std::tuple<std::shared_ptr<AstNode>, std::shared_ptr<AstNode>>;

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp