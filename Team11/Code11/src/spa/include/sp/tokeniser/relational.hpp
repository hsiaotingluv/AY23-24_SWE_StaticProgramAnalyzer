#pragma once

#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include <array>

namespace tokenizer {

class LessThanTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "<", TokenType::LessThan);
    }
};

class LessThanEqualTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "<=", TokenType::LessThanEqual);
    }
};

class GreaterThanTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, ">", TokenType::GreaterThan);
    }
};

class GreaterThanEqualTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, ">=", TokenType::GreaterThanEqual);
    }
};

class DoubleEqualTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "==", TokenType::DoubleEqual);
    }
};

class NotEqualTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "!=", TokenType::NotEqual);
    }
};

class AssignmentTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return tokenize_string(input, "=", TokenType::Assignment);
    }
};

class SPRelationalTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 7>{
        std::make_shared<LessThanEqualTokenizer>(),    std::make_shared<LessThanTokenizer>(),
        std::make_shared<GreaterThanEqualTokenizer>(), std::make_shared<GreaterThanTokenizer>(),
        std::make_shared<DoubleEqualTokenizer>(),      std::make_shared<NotEqualTokenizer>(),
        std::make_shared<AssignmentTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer