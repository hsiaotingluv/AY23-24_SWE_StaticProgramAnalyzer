#pragma once

#include "common/ast/ast.hpp"
#include "sp/parser/factor_parser.hpp"
#include "sp/parser/parser.hpp"

using namespace tokenizer;

namespace sp {

/**
 * @class TermParser
 * @brief Parses Term expressions in the SIMPLE language. *
 *
 * @note  * T: T*F | T/F | T%F | F
 * Removing left recursion, we get a new grammar
 * T: FT'
 * T': e | *fT' | /fT' | %fT'
 */
class TermParser : public Parser {
    FactorParser factor_parser;

    /**
     * Parses the prime of an Term statement.
     * @param token_start Start of the Term statement.
     * @param token_end End of the Term statement.
     * @return Parsed term of the Term statement.
     */
    auto parseTermPrime(Iterator& token_start, const Iterator& token_end)
        -> std::tuple<std::shared_ptr<AstNode>, std::shared_ptr<AstNode>>;

  public:
    /**
     * Parses a Term statement.
     * @param token_start Start of the Term statement.
     * @param token_end End of the Term statement.
     * @return Parsed Term statement.
     */
    auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> override;
};
} // namespace sp