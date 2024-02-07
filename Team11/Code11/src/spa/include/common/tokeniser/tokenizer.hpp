#pragma once

#include "common/tokeniser/token_types.hpp"
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace tokenizer {
using TokeniserInput = std::string;

struct Token {
    TokenType T;
    TokeniserInput content;

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        auto enum_counter = static_cast<int>(token.T);
        os << "Token(" << (token.T == TokenType::Newline ? "\\n" : token.content) << ") - " << enum_counter;
        return os;
    };
};

class Tokenizer {
  protected:
    using TokeniserSuccess = std::tuple<Token, TokeniserInput>;
    using TokeniserOutput = std::optional<TokeniserSuccess>;

    static auto tokenize_string(TokeniserInput input, std::string token, TokenType token_type) -> TokeniserOutput;

    template <typename Iterator>
    static auto one_of(const TokeniserInput& input, Iterator tokenisers_start, const Iterator& tokenisers_end)
        -> TokeniserOutput;

    static auto zero_or_more(const TokeniserInput& input, const std::shared_ptr<Tokenizer>& tokenisers,
                             TokenType token_type) -> TokeniserOutput;

    template <typename Iterator>
    static auto all_of(const TokeniserInput& input, Iterator tokenisers_start, const Iterator& tokenisers_end)
        -> TokeniserOutput;

  public:
    virtual ~Tokenizer() = default;

    [[nodiscard]] virtual auto tokenize(const TokeniserInput& input) const -> TokeniserOutput = 0;
};

template <typename Iterator>
auto Tokenizer::one_of(const TokeniserInput& input, Iterator tokenisers_start, const Iterator& tokenisers_end)
    -> TokeniserOutput {
    for (auto it = tokenisers_start; it != tokenisers_end; it++) {
        const auto result = (*it)->tokenize(input);
        if (result.has_value()) {
            return result;
        }
    }
    return std::nullopt;
}

template <typename Iterator>
auto Tokenizer::all_of(const TokeniserInput& input, Iterator tokenisers_start, const Iterator& tokenisers_end)
    -> TokeniserOutput {
    auto successes = std::vector<Token>{};
    auto remaining_input = input;

    for (auto it = tokenisers_start; it != tokenisers_end; it++) {
        const auto result = (*it)->tokenize(remaining_input);
        if (!result.has_value()) {
            return std::nullopt;
        }
        const auto& [token, rest] = result.value();
        successes.push_back(token);
        remaining_input = std::get<1>(result.value());
    }

    const auto type_combinator = [](const Token& a, const Token& b) -> Token {
        return Token{a.T, a.content + b.content};
    };

    const auto final_token =
        std::accumulate(std::next(successes.begin()), successes.end(), successes[0], type_combinator);
    return std::make_tuple(final_token, remaining_input);
}

} // namespace tokenizer
