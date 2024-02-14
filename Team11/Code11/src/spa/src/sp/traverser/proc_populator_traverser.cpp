#include "sp/traverser/proc_populator_traverser.hpp"
#include "common/ast/procedure_ast.hpp"
#include <memory>

namespace sp {

auto ProcedurePopulatorTraverser::is_proc_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<ProcedureNode>(node) != nullptr);
}

auto ProcedurePopulatorTraverser::traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> {
    if (is_proc_node(node)) {
        auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        write_facade->add_procedure(proc_node->proc_name);
    }

    for (const auto& child : node->get_children()) {
        traverse(child);
    }

    return node;
}
} // namespace sp
