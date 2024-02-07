#pragma once

#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"

namespace tokenizer {
class LParenTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "(", TokenType::LParen);
    }
};

class RParenTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, ")", TokenType::RParen);
    }
};

class SemicolonTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, ";", TokenType::Semicolon);
    }
};

} // namespace tokenizer
