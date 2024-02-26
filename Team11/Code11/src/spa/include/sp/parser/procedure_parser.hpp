#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/statement_list_parser.hpp"

using namespace tokenizer;

namespace sp {

/**
 * @class ProcedureParser
 * @brief Parses procedure expressions in the SIMPLE language.
 *
 * @note procedure: 'procedure' proc_name '{' stmtLst '}'
 */
class ProcedureParser : public Parser {
    StatementListParser statement_list_parser;

  public:
    /**
     * Parses a procedure expression.
     * @param token_start Start of the procedure expression.
     * @param token_end End of the procedure expression.
     * @return Parsed procedure expression.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp