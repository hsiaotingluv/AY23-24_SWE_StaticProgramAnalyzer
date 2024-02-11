#pragma once

#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"

namespace tokenizer {

/**
 * @brief Tokenizer class for the left parenthesis.
 */
class LParenTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "(", TokenType::LParen);
    }
};

/**
 * @brief Tokenizer class for the right parenthesis.
 */
class RParenTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, ")", TokenType::RParen);
    }
};

/**
 * @brief Tokenizer class for the semicolon.
 */
class SemicolonTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, ";", TokenType::Semicolon);
    }
};

} // namespace tokenizer
