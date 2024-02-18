#include "sp/traverser/assignment_populator_traverser.hpp"
#include "common/ast/binary_node_ast.hpp"
#include "common/ast/factor_ast.hpp"

namespace sp {
auto AssignmentPopulatorTraverser::is_assignment_node(const std::shared_ptr<AstNode>& node) -> bool {
    return std::dynamic_pointer_cast<AssignmentNode>(node) != nullptr;
}

auto AssignmentPopulatorTraverser::is_binop_node(const std::shared_ptr<AstNode>& node) -> bool {
    return std::dynamic_pointer_cast<BinopNode>(node) != nullptr;
}

auto AssignmentPopulatorTraverser::generate_postfix_expr(const std::shared_ptr<AstNode>& expression_node)
    -> std::string {
    auto casted_node = std::dynamic_pointer_cast<Expr>(expression_node);
    if (casted_node == nullptr) {
        throw std::runtime_error("Non-expression node is in the rhs of assignment");
    }

    if (is_binop_node(expression_node)) {
        auto binop_node = std::dynamic_pointer_cast<BinopNode>(expression_node);
        auto left = generate_postfix_expr(binop_node->left);
        auto right = generate_postfix_expr(binop_node->right);
        return left + " " + right + " " + casted_node->get_postfix_token();
    } else {
        return casted_node->get_postfix_token();
    }
}

auto AssignmentPopulatorTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>&)
    -> std::shared_ptr<AstNode> {
    if (is_assignment_node(node)) {
        auto assignment_node = std::dynamic_pointer_cast<AssignmentNode>(node);
        auto lhs = std::dynamic_pointer_cast<VarNode>(assignment_node->variable)->name;
        auto rhs = generate_postfix_expr(assignment_node->expr);
        write_facade->add_assignment(std::to_string(assignment_node->get_statement_number()), lhs, rhs);
    } else {
        for (const auto& child : node->get_children()) {
            traverse(child, {});
        }
    }

    return node;
}
} // namespace sp