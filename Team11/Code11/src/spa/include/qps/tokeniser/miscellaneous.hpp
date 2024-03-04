#pragma once
#include "common/tokeniser/tokenizer.hpp"

#include <array>

namespace tokenizer {
class DotTokenizer : public tokenizer::Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, ".", TokenType::Dot);
    }
};

class HashTokenizer : public tokenizer::Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "#", TokenType::Hash);
    }
};

class SingleEqualTokenizer : public tokenizer::Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "=", TokenType::SingleEqual);
    }
};

class MiscellaneousTokenizer : public tokenizer::Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 3>{
        std::make_shared<DotTokenizer>(), std::make_shared<HashTokenizer>(), std::make_shared<SingleEqualTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};

} // namespace tokenizer
