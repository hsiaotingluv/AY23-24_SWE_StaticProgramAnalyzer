#pragma once
#include "common/tokeniser/tokenizer.hpp"

#include <array>

namespace tokenizer {
class DotTokenizer final : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, ".", TokenType::Dot);
    }
};

class SingleEqualTokenizer final : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "=", TokenType::SingleEqual);
    }
};

class MiscellaneousTokenizer final : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 2>{
        std::make_shared<DotTokenizer>(), std::make_shared<SingleEqualTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};

} // namespace tokenizer
