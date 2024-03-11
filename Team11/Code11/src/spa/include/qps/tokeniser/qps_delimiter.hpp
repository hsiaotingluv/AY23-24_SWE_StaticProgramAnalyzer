#pragma once
#include "common/tokeniser/delimiter.hpp"
#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/tokeniser/relational.hpp"

#include <array>

namespace tokenizer {

static constexpr auto Delimiter =
    std::array<TokenType, 7>{TokenType::Comma,     TokenType::DQuote,   TokenType::LParen,     TokenType::RParen,
                             TokenType::Semicolon, TokenType::LessThan, TokenType::GreaterThan};

class CommaTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, ",", TokenType::Comma);
    }
};

class DQuoteTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "\"", TokenType::DQuote);
    }
};

class QPSDelimiterTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 7>{
        std::make_shared<LParenTokenizer>(),     std::make_shared<RParenTokenizer>(),
        std::make_shared<SemicolonTokenizer>(),  std::make_shared<CommaTokenizer>(),
        std::make_shared<DQuoteTokenizer>(),     std::make_shared<LessThanTokenizer>(),
        std::make_shared<GreaterThanTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer