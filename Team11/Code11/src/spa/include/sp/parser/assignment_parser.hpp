#pragma once

#include "sp/parser/parser.hpp"
#include "sp/parser/name_parser.hpp"
#include "sp/parser/expr_parser.hpp"
#include <memory>
#include <vector>

using namespace tokenizer;

namespace sp {
/*
 * Therefore, let V = var_name, A = assign and E = expr
 * A: V = E
 */
class AssignmentParser : public Parser {
    auto parseExprPrime(Iterator& token_start, const Iterator& token_end)
        -> std::tuple<std::shared_ptr<AstNode>, std::shared_ptr<AstNode>>;

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp