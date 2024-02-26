#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"

#include <memory>

using namespace tokenizer;

namespace sp {

/*
 *
 */
/**
 * @class NameParser
 * @brief Parses Name expressions in the SIMPLE language.
 *
 * @note var_name, proc_name: NAME
 */
class NameParser : public Parser {
  public:
    /**
     * Parses a Name expression.
     * @param token_start Start of the name expression.
     * @param token_end End of the name expression.
     * @return Parsed name expression.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp