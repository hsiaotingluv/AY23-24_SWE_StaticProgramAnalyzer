#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"

#include <memory>

namespace sp {

/**
 * @class ConstantParser
 * @brief Parses constant expressions in the SIMPLE language.
 * @note const_value: INTEGER
 *
 */
class ConstantParser : public Parser {
  public:
    /**
     * Parses a constant expression.
     * @param token_start Start of the constant expression.
     * @param token_end End of the constant expression.
     * @return Parsed constant expression.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp