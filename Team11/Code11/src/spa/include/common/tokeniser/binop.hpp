#pragma once

#include "common/tokeniser/tokenizer.hpp"

#include <array>

namespace tokenizer {

/**
 * @brief Tokenizer class for the addition operator.
 */
class AddTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "+", TokenType::Add);
    }
};

/**
 * @brief Tokenizer class for the subtraction operator.
 */
class SubTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "-", TokenType::Sub);
    }
};

/**
 * @brief Tokenizer class for the multiplication operator.
 */
class MulTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "*", TokenType::Mul);
    }
};

/**
 * @brief Tokenizer class for the division operator.
 */
class DivTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "/", TokenType::Div);
    }
};

/**
 * @brief Tokenizer class for the modulus operator.
 */
class ModTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "%", TokenType::Mod);
    }
};

/**
 * @brief Tokenizer class for the all Binary Operators.
 */
class BinopTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 5>{
        std::make_shared<AddTokenizer>(), std::make_shared<SubTokenizer>(), std::make_shared<MulTokenizer>(),
        std::make_shared<DivTokenizer>(), std::make_shared<ModTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer
