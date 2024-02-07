#pragma once

#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include <array>
#include <memory>

namespace tokenizer {
class ZeroTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "0", TokenType::Integer);
    }
};

class NotZeroDigitTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        if (input.empty() || !std::isdigit(input[0]) || input[0] == '0') {
            return std::nullopt;
        }

        return std::make_tuple(Token{TokenType::Integer, std::string{input[0]}}, input.substr(1));
    }
};

class DigitTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 2>{
        std::make_shared<NotZeroDigitTokenizer>(), std::make_shared<ZeroTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};

class SomeDigitTokenizer : public Tokenizer {
  private:
    static inline const auto digit_tokenizer = std::make_shared<DigitTokenizer>();

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return zero_or_more(input, digit_tokenizer, TokenType::Integer);
    }
};

class NoLeadingZeroTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 2>{
        std::make_shared<NotZeroDigitTokenizer>(), std::make_shared<SomeDigitTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return all_of(input, tokenizers.begin(), tokenizers.end());
    }
};

class IntegerTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 2>{
        std::make_shared<NoLeadingZeroTokenizer>(), std::make_shared<ZeroTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer