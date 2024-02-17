#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/statement_parser.hpp"

#include <memory>

#include "sp/parser/expr_parser.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/statement_keyword_const.hpp"
#include "sp/parser/statement_parser.hpp"

using namespace tokenizer;

namespace sp {

/**
 * @class StatementListParser
 * @brief Parses StatementList in the SIMPLE language. *
 *
 * @note stmtLst: stmt+
 */
class StatementListParser : public Parser {
    StatementParser statement_parser;

  public:
    /**
     * Parses a StatementList.
     * @param token_start Start of the StatementList.
     * @param token_end End of the StatementList.
     * @return Parsed StatementList.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp