#pragma once
#include "common/tokeniser/tokenizer.hpp"
#include "qps/parser/entities/synonym.hpp"

using Token = tokenizer::Token;

namespace qps {
auto is_delimiter(const Token& token) -> bool;
auto is_string(const Token& token) -> bool;
auto is_integer(const Token& token) -> bool;
auto is_wildcard(const Token& token) -> bool;
auto is_binary(const Token& token) -> bool;
auto is_stmt_ref(const Token& token) -> bool;
auto is_keyword(const Token& token, const std::string& keyword) -> bool;

auto is(const Token& token, std::string content) -> bool;

auto get_stmt_ref(const Synonyms& declared, const Token& token) -> std::optional<StmtRef>;

template <char c>
auto is_char(const Token& token) -> bool {
    return token.content.size() == 1 && token.content[0] == c;
}

const auto is_open_bracket = [](const Token& token) -> bool {
    return is_delimiter(token) && is_char<'('>(token);
};

const auto is_close_bracket = [](const Token& token) -> bool {
    return is_delimiter(token) && is_char<')'>(token);
};

const auto is_comma = [](const Token& token) -> bool {
    return is_delimiter(token) && is_char<','>(token);
};

const auto is_semicolon = [](const Token& token) -> bool {
    return is_delimiter(token) && is_char<';'>(token);
};

const auto is_star = [](const Token& token) -> bool {
    return is_binary(token) && is_char<'*'>(token);
};

const auto is_open_quote = [](const Token& token) -> bool {
    return is_delimiter(token) && is_char<'"'>(token);
};

const auto is_close_quote = [](const Token& token) -> bool {
    return is_delimiter(token) && is_char<'"'>(token);
};

} // namespace qps