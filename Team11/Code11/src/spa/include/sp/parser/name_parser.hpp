#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"

#include <memory>

using namespace tokenizer;

namespace sp {

/*
 * var_name, proc_name: NAME
 */
class NameParser : public Parser {
  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp