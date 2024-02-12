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

/**
 * @brief Represents a token.
 *
 * This struct is used to store information about a token, including its type and content.
 */
struct Token {
    TokenType T;
    TokeniserInput content;

    /**
     * @brief Overloaded stream insertion operator for printing the token.
     *
     * This operator allows the token to be printed in a human-readable format.
     * If the content of the token is a tab character ("\t"), it will be displayed as "Token(\t) - <enum_counter>".
     * If the content of the token is a newline character ("\n"), it will be displayed as "Token(\n) - <enum_counter>".
     * Otherwise, it will be displayed as "Token(<content>) - <enum_counter>".
     *
     * @param os The output stream.
     * @param token The token to be printed.
     * @return The output stream after printing the token.
     */
    friend auto operator<<(std::ostream& os, const Token& token) -> std::ostream& {
        auto enum_counter = static_cast<int>(token.T);
        if (token.content == "\t") {
            os << "Token(\\t) - " << enum_counter;
        } else if (token.content == "\n") {
            os << "Token(\\n) - " << enum_counter;
        } else {
            os << "Token(" << token.content << ") - " << enum_counter;
        }
        return os;
    };
};

/**
 * @class Tokenizer
 * @brief Base class for tokenizers used in the SPA system.
 *
 * The Tokenizer class provides a common interface for tokenizers used in the SPA system.
 * Tokenizers are responsible for converting input strings into tokens, which are used for
 * further analysis and processing.
 */

class Tokenizer {
  protected:
    using TokeniserSuccess = std::tuple<Token, TokeniserInput>;
    using TokeniserOutput = std::optional<TokeniserSuccess>;

    /**
     * @brief Tokenizes a string based on a given token and token type.
     *
     * This function tokenizes the input string based on the given token and token type.
     *
     * @param input The input string to tokenize.
     * @param token The token to search for in the input string.
     * @param token_type The type of token to assign to the matched token.
     * @return An optional tuple containing the token and the remaining input, or an empty optional if the token is not
     * found.
     */
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

    /**
     * @brief Tokenizes the input string.
     *
     * This function tokenizes the input string and returns an optional tuple containing
     * the token and the remaining input. If the tokenization is successful, the tuple
     * will be populated with the token and the remaining input. If the tokenization fails,
     * an empty optional will be returned.
     *
     * @param input The input string to tokenize.
     * @return An optional tuple containing the token and the remaining input, or an empty optional if the tokenization
     * fails.
     */
    [[nodiscard]] virtual auto tokenize(const TokeniserInput& input) const -> TokeniserOutput = 0;
};

template <typename Iterator>
auto Tokenizer::one_of(const TokeniserInput& input, Iterator tokenisers_start, const Iterator& tokenisers_end)
    -> TokeniserOutput {
    for (auto it = tokenisers_start; it != tokenisers_end; it++) {
        auto result = (*it)->tokenize(input);
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
