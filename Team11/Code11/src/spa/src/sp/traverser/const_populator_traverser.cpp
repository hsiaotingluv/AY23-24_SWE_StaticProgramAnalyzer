#include "sp/traverser/const_populator_traverser.hpp"
#include "sp/parser/ast/factor_ast.hpp"
#include <memory>

namespace sp {

auto ConstPopulatorTraverser::is_const_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<ConstantNode>(node) != nullptr);
}

auto ConstPopulatorTraverser::traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> {
    if (is_const_node(node)) {
        auto const_node = std::dynamic_pointer_cast<ConstantNode>(node);
        write_facade->addConstant(std::to_string(const_node->integer));
    }

    for (const auto& child : node->get_children()) {
        traverse(child);
    }

    return node;
}
} // namespace sp
