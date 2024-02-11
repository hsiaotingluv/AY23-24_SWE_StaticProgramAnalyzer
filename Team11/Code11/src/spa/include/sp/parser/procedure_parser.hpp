#pragma once

#include "sp/parser/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/statement_list_parser.hpp"

#include <memory>

using namespace tokenizer;

namespace sp {

/*
 * procedure: 'procedure' proc_name '{' stmtLst '}'
 */
class ProcedureParser : public Parser {
    StatementListParser statement_list_parser;

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp