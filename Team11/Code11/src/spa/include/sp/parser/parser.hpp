#pragma once

#include "common/ast/ast.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/exception/parsing_error.hpp"

#include <memory>
#include <vector>

using namespace tokenizer;

namespace sp {

/**
 * @class Parser
 * @brief Base class for parsing tokens into an abstract syntax tree (AST).
 */
class Parser {
  public:
    using Iterator = std::vector<Token>::const_iterator;

  protected:
    /**
     * @brief Checks if there is a token available for parsing.
     * @param token_start The iterator pointing to the current token.
     * @param token_end The iterator pointing to the end of the token sequence.
     * @throws ParsingError if no token is available for parsing.
     */
    static auto check_has_token(const Iterator& token_start, const Iterator& token_end) -> void {
        if (token_start == token_end) {
            throw ParsingError("Unable to parse token");
        };
    }

    /**
     * @brief Retrieves the next token for parsing and advances the iterator.
     * @param token_start The iterator pointing to the current token.
     * @return The next token to be parsed.
     */
    static auto get_next_token(Iterator& token_start) -> Token {
        auto token = *token_start;
        std::advance(token_start, 1);
        return token;
    }

    /**
     * @brief Retrieves the next token for parsing without advancing the iterator.
     * @param token_start The iterator pointing to the current token.
     * @return The next token to be parsed.
     */
    static auto peek_next_token(Iterator& token_start) -> Token {
        auto token = *token_start;
        return token;
    }

  public:
    /**
     * @brief Parses a sequence of tokens into an abstract syntax tree (AST).
     * @param token_start The iterator pointing to the current token.
     *        This iterator will be updated to point to the next token after parsing.
     * @param token_end The iterator pointing to the end of the token sequence.
     * @return A shared pointer to the root node of the parsed AST.
     */
    virtual auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> = 0;
    virtual ~Parser() = default;
};
} // namespace sp