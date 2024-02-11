#pragma once

#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"

#include <array>

namespace tokenizer {

/**
 * @brief Tokenizer class for the whitespace character.
 */
class WhitespaceTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, " ", TokenType::Junk);
    }
};

/**
 * @brief Tokenizer class for the newline character.
 */
class NewlineTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "\n", TokenType::Junk);
    }
};

/**
 * @brief Tokenizer class for the tab character.
 */
class TabTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "\t", TokenType::Junk);
    }
};

/**
 * @brief Tokenizer class for all irrelevant (junk) character.
 */
class JunkTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 3>{
        std::make_shared<NewlineTokenizer>(), std::make_shared<WhitespaceTokenizer>(),
        std::make_shared<TabTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer
