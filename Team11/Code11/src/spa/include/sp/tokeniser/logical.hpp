#pragma once

#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include <array>

namespace tokenizer {

/**
 * @brief Tokenizer class for the AND operator.
 */
class LAndTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "&&", TokenType::LAnd);
    }
};

/**
 * @brief Tokenizer class for the OR operator.
 */
class LOrTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "||", TokenType::LOr);
    }
};

/**
 * @brief Tokenizer class for the NOT operator.
 */
class LNotTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "!", TokenType::LNot);
    }
};

/**
 * @brief Tokenizer class for the SourceProcessor Logical operator.
 */
class SPLogicalTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 3>{
        std::make_shared<LNotTokenizer>(), std::make_shared<LOrTokenizer>(), std::make_shared<LAndTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer