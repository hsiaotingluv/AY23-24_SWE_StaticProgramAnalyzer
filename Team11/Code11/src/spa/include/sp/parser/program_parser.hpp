#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/procedure_parser.hpp"

namespace sp {

/**
 * @class ProgramParser
 * @brief Parse the Root Node of the SIMPLE language.
 *
 * @note program: procedure+
 * A: V = E
 */
class ProgramParser : public Parser {
    ProcedureParser procedure_parser;

  public:
    /**
     * Parses the root node of the SIMPLE language.
     * @param token_start Start of the program expression.
     * @param token_end End of the program expression.
     * @return Parsed program expression.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp