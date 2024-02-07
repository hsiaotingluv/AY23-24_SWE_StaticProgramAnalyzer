#pragma once

#include "common/tokeniser/token_types.hpp"

#include <unordered_map>

namespace qps {
using TokenType = tokenizer::TokenType;

static const auto bop_precedence = std::unordered_map<TokenType, int>{
    {{TokenType::Add, 1}, {TokenType::Sub, 1}, {TokenType::Mul, 2}, {TokenType::Div, 2}, {TokenType::Mod, 2}}};

static inline auto get_precedence(TokenType T) -> int {
    if (bop_precedence.find(T) == bop_precedence.end()) {
        return -1;
    }

    return bop_precedence.at(T);
}

} // namespace qps