#include "sp/parser/program_parser.hpp"
#include "common/ast/program_ast.hpp"

using namespace tokenizer;

namespace sp {
auto ProgramParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);
    std::vector<std::shared_ptr<AstNode>> procedure_nodes;

    // At least parse 1 procedure
    auto procedure_tree = procedure_parser.parse(token_start, token_end);
    procedure_nodes.push_back(procedure_tree);

    for (;;) {
        auto backup_token_start = token_start;
        try {
            procedure_tree = procedure_parser.parse(token_start, token_end);
            procedure_nodes.push_back(procedure_tree);
        } catch (const ParsingError& pe) {
            token_start = backup_token_start;
            auto next_token = peek_next_token(token_start);
            if (next_token.T != TokenType::Done) {
                throw ParsingError("Expecting done token when parsing program, fine another token");
            }

            return std::make_shared<ProgramNode>(procedure_nodes);
        }
    }
}

} // namespace sp
