#include "sp/parser/procedure_parser.hpp"
#include "common/ast/procedure_ast.hpp"
#include "sp/parser/statement_keyword_const.hpp"

using namespace tokenizer;

namespace sp {
auto ProcedureParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);

    auto procedure_string_token = get_next_token(token_start);
    if (procedure_string_token.T != TokenType::String || procedure_string_token.content != PROCEDURE) {
        throw ParsingError("Expecting procedure keyword in procedure but found other token");
    }

    auto procedure_name = get_next_token(token_start);
    if (procedure_string_token.T != TokenType::String) {
        throw ParsingError("Expecting procedure name in procedure but found other token");
    }

    auto lcurly_token = get_next_token(token_start);
    if (lcurly_token.T != TokenType::LCurly) {
        throw ParsingError("Expecting { keyword in procedure but found other token");
    }

    auto stmt_list_tree = statement_list_parser.parse(token_start, token_end);
    auto statement_list_node = std::dynamic_pointer_cast<StatementListNode>(stmt_list_tree);

    auto rcurly_token = get_next_token(token_start);
    if (rcurly_token.T != TokenType::RCurly) {
        throw ParsingError("Expecting } keyword in procedure but found other token");
    }

    return std::make_shared<ProcedureNode>(procedure_name.content, statement_list_node);
}

} // namespace sp
