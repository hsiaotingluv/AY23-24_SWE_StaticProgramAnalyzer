#include "common/tokeniser/tokenizer.hpp"
#include <optional>

namespace tokenizer {
auto Tokenizer::tokenize_string(TokeniserInput input, std::string token, TokenType token_type)
    -> Tokenizer::TokeniserOutput {
    if (input.length() < token.length()) {
        return std::nullopt;
    }

    for (int i = 0; i < token.length(); i++) {
        if (input[i] != token[i]) {
            return std::nullopt;
        }
    }

    return std::make_tuple(Token{token_type, token}, input.substr(token.length()));
}

auto Tokenizer::zero_or_more(const TokeniserInput& input, const std::shared_ptr<Tokenizer>& tokenisers, TokenType type)
    -> Tokenizer::TokeniserOutput {
    const auto result = tokenisers->tokenize(input);
    if (!result.has_value()) {
        return std::make_tuple(Token{type, ""}, input);
    }

    auto [success, rest] = result.value();
    while (true) {
        const auto maybe_next = tokenisers->tokenize(rest);
        if (!maybe_next.has_value()) {
            return std::make_tuple(success, rest);
        }

        const auto& [next_token, rem] = maybe_next.value();
        success = Token{next_token.T, success.content + next_token.content};
        rest = rem;
    }
}

} // namespace tokenizer
