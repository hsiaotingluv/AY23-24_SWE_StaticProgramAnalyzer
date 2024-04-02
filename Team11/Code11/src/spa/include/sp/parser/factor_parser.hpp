#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/constant_parser.hpp"
#include "sp/parser/expr_parser.hpp"
#include "sp/parser/name_parser.hpp"

namespace sp {

/**
 * @class FactorParser
 * @brief Parses Factor expressions in the SIMPLE language. *
 *
 * @note factor: var_name | const_value | '(' expr ')'
 *
 */
class FactorParser : public Parser {
    NameParser name_parser;
    ConstantParser constant_parser;
    ExprParser expr_parser;

  public:
    /**
     * Parses a factor expression.
     * @param token_start Start of the factor expression.
     * @param token_end End of the factor expression.
     * @return Parsed factor expression.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};

} // namespace sp