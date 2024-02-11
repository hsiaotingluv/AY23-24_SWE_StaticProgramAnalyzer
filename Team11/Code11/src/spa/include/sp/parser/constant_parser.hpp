#pragma once

#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include <memory>
#include <vector>

using namespace tokenizer;

namespace sp {

/*
 * const_value: INTEGER
 */
class ConstantParser : public Parser {
  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp