#pragma once

#include "common/ast/ast.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/exception/parsing_error.hpp"

#include <memory>
#include <vector>

using namespace tokenizer;

namespace sp {

class Parser {
  public:
    using Iterator = std::vector<Token>::const_iterator;

  protected:
    static auto check_has_token(const Iterator& token_start, const Iterator& token_end) -> void {
        if (token_start == token_end) {
            throw ParsingError("Unable to parse token");
        };
    };

    static auto get_next_token(Iterator& token_start) -> Token {
        auto token = *token_start;
        std::advance(token_start, 1);
        return token;
    }

    static auto peek_next_token(Iterator& token_start) -> Token {
        auto token = *token_start;
        return token;
    }

  public:
    virtual auto parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> = 0;
    virtual ~Parser() = default;
};
} // namespace sp