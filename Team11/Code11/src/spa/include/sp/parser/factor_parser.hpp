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
 * factor: var_name | const_value | '(' expr ')'
 */
class FactorParser : public Parser {
    NameParser name_parser;
    ConstantParser constant_parser;
    ExprParser expr_parser;

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp