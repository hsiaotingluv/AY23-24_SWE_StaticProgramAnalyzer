#pragma once

#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/ast/ast.hpp"
#include "sp/parser/expr_parser.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/statement_keyword_const.hpp"
#include "sp/parser/statement_parser.hpp"
#include <memory>
#include <vector>

using namespace tokenizer;

namespace sp {

/*
 * stmtLst: stmt+
 */
class StatementListParser : public Parser {
    StatementParser statement_parser;

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp