#pragma once

#include "common/tokeniser/delimiter.hpp"
#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"

#include <array>

namespace tokenizer {
/**
 * @brief Tokenizer class for the left curly bracket character.
 */
class LCurlyTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "{", TokenType::LCurly);
    }
};

/**
 * @brief Tokenizer class for the right curly bracket character.
 */
class RCurlyTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "}", TokenType::RCurly);
    }
};

/**
 * @brief Tokenizer class for the SourceProcessor delimiter character.
 */
class SPDelimiterTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 5>{
        std::make_shared<LParenTokenizer>(), std::make_shared<RParenTokenizer>(), std::make_shared<LCurlyTokenizer>(),
        std::make_shared<RCurlyTokenizer>(), std::make_shared<SemicolonTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};

} // namespace tokenizer