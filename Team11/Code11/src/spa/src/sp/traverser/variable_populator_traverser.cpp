#include "sp/traverser/variable_populator_traverser.hpp"
#include "sp/parser/ast/factor_ast.hpp"
#include <memory>

namespace sp {

auto VarPopulatorTraverser::is_var_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<VarNode>(node) != nullptr);
}

auto VarPopulatorTraverser::traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> {
    if (is_var_node(node)) {
        auto var_node = std::dynamic_pointer_cast<VarNode>(node);
        write_facade->addVariable(var_node->name);
    }

    for (const auto& child : node->get_children()) {
        traverse(child);
    }

    return node;
}
} // namespace sp
