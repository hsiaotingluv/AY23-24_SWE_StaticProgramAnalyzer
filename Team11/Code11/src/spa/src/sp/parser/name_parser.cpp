#include "sp/parser/name_parser.hpp"
#include "sp/parser/ast/factor_ast.hpp"

namespace sp {

auto NameParser::parse(Iterator& token_start, const Iterator& token_end) -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);

    auto next_token = get_next_token(token_start);

    if (next_token.T != TokenType::String) {
        throw ParsingError("Token found is not of String type");
    }

    return std::make_shared<VarNode>(next_token.content);
}

} // namespace sp
