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
class HorizontalTabTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "\t", TokenType::Junk);
    }
};

class VerticalTabTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "\v", TokenType::Junk);
    }
};

class FormFeedTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "\f", TokenType::Junk);
    }
};

class CarriageReturnTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "\r", TokenType::Junk);
    }
};

/**
 * @brief Tokenizer class for all irrelevant (junk) character.
 */
class JunkTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 6>{
        std::make_shared<NewlineTokenizer>(),       std::make_shared<WhitespaceTokenizer>(),
        std::make_shared<VerticalTabTokenizer>(),   std::make_shared<FormFeedTokenizer>(),
        std::make_shared<HorizontalTabTokenizer>(), std::make_shared<CarriageReturnTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer
