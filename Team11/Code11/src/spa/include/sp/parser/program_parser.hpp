#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/procedure_parser.hpp"

#include <memory>

using namespace tokenizer;

namespace sp {

/*
 * program: procedure+
 */
class ProgramParser : public Parser {
    ProcedureParser procedure_parser;

  public:
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp