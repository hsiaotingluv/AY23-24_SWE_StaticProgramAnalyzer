#pragma once

#include "common/tokeniser/integer.hpp"
#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"

namespace tokenizer {

/**
 * @brief Tokenizer class for the letter character.
 */
class LetterTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        if (input.empty() || std::isalpha(input[0]) == 0) {
            return std::nullopt;
        }

        return std::make_tuple(Token{TokenType::String, std::string{input[0]}}, input.substr(1));
    }
};

/**
 * @brief Tokenizer class for the digit or letter character.
 */
class DigitOrLetterTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 2>{
        std::make_shared<DigitTokenizer>(), std::make_shared<LetterTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};

/**
 * @brief Tokenizer class for muliple numerical digits or letters.
 */
class SomeDigitOrLetterTokenizer : public Tokenizer {
  private:
    static inline const auto digit_tokenizer = std::make_shared<DigitOrLetterTokenizer>();

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return zero_or_more(input, digit_tokenizer, TokenType::String);
    }
};

/**
 * @brief Tokenizer class for all String Tokens.
 */
class StringTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 2>{
        std::make_shared<LetterTokenizer>(), std::make_shared<SomeDigitOrLetterTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return all_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer