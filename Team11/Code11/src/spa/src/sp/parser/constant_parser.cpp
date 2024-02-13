#include "sp/parser/constant_parser.hpp"
#include "common/ast/factor_ast.hpp"

namespace sp {

auto ConstantParser::parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);

    auto next_token = get_next_token(token_start);

    if (next_token.T != TokenType::Integer) {
        throw ParsingError("Token found is not of Integer type");
    }

    int integer_value = std::stoi(next_token.content);
    return std::make_shared<ConstantNode>(integer_value);
}

} // namespace sp
