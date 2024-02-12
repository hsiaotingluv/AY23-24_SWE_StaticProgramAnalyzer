#include "sp/parser/ast/statement_ast.hpp"
#include "sp/parser/statement_parser.hpp"

namespace sp {

// print: 'print' var_name';'
auto StatementParser::parsePrintStmt(Parser::Iterator& token_start, const Token& next_token)
    -> std::shared_ptr<AstNode> {
    if (next_token.T != TokenType::String) {
        throw ParsingError("Expecting varname for print but found other token");
    }

    auto semicolon = get_next_token(token_start);
    if (semicolon.T != TokenType::Semicolon) {
        throw ParsingError("Expecting ; in assignment but found other token");
    }

    return std::make_shared<PrintNode>(next_token.content);
}
} // namespace sp