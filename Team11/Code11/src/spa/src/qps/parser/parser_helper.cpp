#include "common/tokeniser/token_types.hpp"

#include "qps/parser/parser_helper.hpp"
#include "qps/tokeniser/binop.hpp"
#include "qps/tokeniser/qps_delimiter.hpp"

#include <algorithm>

namespace qps {

using TokenType = tokenizer::TokenType;

auto is_delimiter(const Token& token) -> bool {
    return std::any_of(tokenizer::Delimiter.begin(), tokenizer::Delimiter.end(), [&token](auto&& x) -> bool {
        return x == token.T;
    });
}

auto is_string(const Token& token) -> bool {
    return token.T == TokenType::String;
}

auto is_integer(const Token& token) -> bool {
    return token.T == TokenType::Integer;
}

auto is_wildcard(const Token& token) -> bool {
    return token.T == TokenType::Wildcard;
}

auto is_binary(const Token& token) -> bool {
    return std::any_of(bop_precedence.begin(), bop_precedence.end(), [&token](auto&& x) -> bool {
        return x.first == token.T;
    });
}

auto is_stmt_ref(const Token& token) -> bool {
    return is_string(token) || is_wildcard(token) || is_integer(token);
}

auto is_keyword(const Token& token, const std::string& keyword) -> bool {
    return token.T == TokenType::String && token.content == keyword;
}

auto is_relationship_keyword(const Token& token, const std::string& keyword) -> bool {
    return token.T == TokenType::String && token.content == keyword;
}

auto is(const Token& token, const std::string& content) -> bool {
    return token.content == content;
}

} // namespace qps