#include "sp/traverser/modifies_traverser.hpp"
#include "common/ast/ast.hpp"
#include "common/ast/factor_ast.hpp"
#include "common/ast/statement_ast.hpp"
#include <memory>

namespace sp {

auto ModifiesTraverser::is_read_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<ReadNode>(node) != nullptr);
}

auto ModifiesTraverser::is_assignment_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<AssignmentNode>(node) != nullptr);
}

auto ModifiesTraverser::traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> {
    /*
     * TODO: continue with container, procedure, and proc call
     */
    if (is_assignment_node(node)) {
        auto assignment_node = std::dynamic_pointer_cast<AssignmentNode>(node);
        auto stmt_number = std::to_string(assignment_node->get_statement_number());
        auto var_node = std::dynamic_pointer_cast<VarNode>(assignment_node->variable);
        write_facade->add_statement_modifies_var(stmt_number, var_node->name);
    } else if (is_read_node(node)) {
        auto read_node = std::dynamic_pointer_cast<ReadNode>(node);
        auto stmt_number = std::to_string(read_node->get_statement_number());
        write_facade->add_statement_modifies_var(stmt_number, read_node->variable);
    } else {
        for (const auto& child : node->get_children()) {
            traverse(child);
        }
    }

    return node;
}
} // namespace sp
