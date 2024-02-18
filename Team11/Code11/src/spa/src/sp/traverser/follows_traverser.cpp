#include "sp/traverser/follows_traverser.hpp"
#include "common/ast/statement_list_ast.hpp"

namespace sp {

auto FollowsTraverser::traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>&)
    -> std::shared_ptr<AstNode> {

    const auto is_statement_list = std::dynamic_pointer_cast<StatementListNode>(ast) != nullptr;
    if (is_statement_list) {
        auto statement_list = std::dynamic_pointer_cast<StatementListNode>(ast);
        auto children = statement_list->statements;
        for (auto it = children.begin(); it != children.end() && std::next(it) != children.end(); ++it) {
            auto first_statement = std::dynamic_pointer_cast<StatementNode>(*it);
            auto second_statement = std::dynamic_pointer_cast<StatementNode>(*std::next(it));
            write_facade->add_follows(std::to_string(first_statement->get_statement_number()),
                                      std::to_string(second_statement->get_statement_number()));
        }
    }

    for (const auto& child : ast->get_children()) {
        traverse(child, {});
    }

    return ast;
}
} // namespace sp