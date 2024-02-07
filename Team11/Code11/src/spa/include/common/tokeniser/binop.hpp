#pragma once

#include "common/tokeniser/tokenizer.hpp"

#include <array>

namespace tokenizer {
class AddTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "+", TokenType::Add);
    }
};

class SubTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "-", TokenType::Sub);
    }
};

class MulTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "*", TokenType::Mul);
    }
};

class DivTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "/", TokenType::Div);
    }
};

class ModTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "%", TokenType::Mod);
    }
};

class BinopTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 5>{
        std::make_shared<AddTokenizer>(), std::make_shared<SubTokenizer>(), std::make_shared<MulTokenizer>(),
        std::make_shared<DivTokenizer>(), std::make_shared<ModTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer
