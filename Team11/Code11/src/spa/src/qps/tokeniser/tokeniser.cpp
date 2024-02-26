#include "qps/tokeniser/tokeniser.hpp"

namespace qps {
auto QueryProcessingSystemTokenizer::tokenize(const TokeniserInput& input) const -> TokeniserOutput {
    return one_of(input, tokenizers.begin(), tokenizers.end());
}

auto WildCardTokenizer::tokenize(const TokeniserInput& input) const -> TokeniserOutput {
    if (input.length() < 1 || input[0] != '_') {
        return std::nullopt;
    }

    return std::make_tuple(Token{TokenType::Wildcard, "_"}, input.substr(1));
}

} // namespace qps
