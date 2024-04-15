#include "sp/parser/factor_parser.hpp"

using namespace tokenizer;

namespace sp {
auto FactorParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end) -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);

    auto next_token = peek_next_token(token_start);

    switch (next_token.T) {
    case TokenType::Integer: {
        return constant_parser.parse(token_start, token_end);
    }
    case TokenType::String: {
        return name_parser.parse(token_start, token_end);
    }
    case TokenType::LParen: {
        get_next_token(token_start);
        auto expr_tree = expr_parser.parse(token_start, token_end);
        auto last_token = get_next_token(token_start);

        if (last_token.T != TokenType::RParen) {
            throw ParsingError("Expecting RParen for factor parser but found other token");
        }

        return expr_tree;
    }
    default:
        throw ParsingError("Unhandled token in factor parser");
    }
}

} // namespace sp
