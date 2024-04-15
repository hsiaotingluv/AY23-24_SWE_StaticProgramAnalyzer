#include "sp/parser/statement_list_parser.hpp"
#include "common/ast/statement_list_ast.hpp"

using namespace tokenizer;

namespace sp {
/*
 * Note that for a list of statements to be valid, by the end of the parsing
 * the next token must be } since all stmtList is used in { stmtLst } context,
 * otherwise { read x; 1 + 2; } can be valid as we stop parsing once it gets Parsing Error
 */
auto StatementListParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {

    check_has_token(token_start, token_end);
    std::vector<std::shared_ptr<AstNode>> statement_nodes;

    // At least process 1 statement
    auto statement_tree = statement_parser.parse(token_start, token_end);
    statement_nodes.push_back(statement_tree);

    for (;;) {
        auto backup_token_start = token_start;
        try {
            statement_tree = statement_parser.parse(token_start, token_end);
            statement_nodes.push_back(statement_tree);
        } catch (const ParsingError& pe) {
            token_start = backup_token_start;
            auto next_token = peek_next_token(token_start);
            if (next_token.T != TokenType::RCurly && next_token.T != TokenType::Done) {
                throw ParsingError("Expecting statement list to end with } or Done but found other token");
            }

            return std::make_shared<StatementListNode>(statement_nodes);
        }
    }
}
} // namespace sp