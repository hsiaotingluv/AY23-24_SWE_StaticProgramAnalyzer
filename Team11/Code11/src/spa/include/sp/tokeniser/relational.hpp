#pragma once

#include "common/tokeniser/token_types.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include <array>

namespace tokenizer {

/**
 * @brief Tokenizer class for the Less Than operator.
 */
class LessThanTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "<", TokenType::LessThan);
    }
};

/**
 * @brief Tokenizer class for the Less Than Equal operator.
 */
class LessThanEqualTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "<=", TokenType::LessThanEqual);
    }
};

/**
 * @brief Tokenizer class for the Greater Than operator.
 */
class GreaterThanTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, ">", TokenType::GreaterThan);
    }
};

/**
 * @brief Tokenizer class for the Greater Than Equal operator.
 */
class GreaterThanEqualTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, ">=", TokenType::GreaterThanEqual);
    }
};

/**
 * @brief Tokenizer class for the Double Equal operator.
 */
class DoubleEqualTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "==", TokenType::DoubleEqual);
    }
};

/**
 * @brief Tokenizer class for the Not Equal operator.
 */
class NotEqualTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "!=", TokenType::NotEqual);
    }
};

/**
 * @brief Tokenizer class for the Single Equal (Assignment) operator.
 */
class AssignmentTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, "=", TokenType::Assignment);
    }
};

/**
 * @brief Tokenizer class for all Relational operator.
 */
class SPRelationalTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 7>{
        std::make_shared<LessThanEqualTokenizer>(),    std::make_shared<LessThanTokenizer>(),
        std::make_shared<GreaterThanEqualTokenizer>(), std::make_shared<GreaterThanTokenizer>(),
        std::make_shared<DoubleEqualTokenizer>(),      std::make_shared<NotEqualTokenizer>(),
        std::make_shared<AssignmentTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace tokenizer