#include "sp/traverser/parent_traverser.hpp"

namespace sp {
    
auto ParentTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>&)
    -> std::shared_ptr<AstNode> {
    
    if (NodeTypeChecker::is_if_node(node)) {
        auto if_node = std::dynamic_pointer_cast<IfNode>(node);
        if_node->populate_pkb_parent(write_facade);
    } else if (NodeTypeChecker::is_while_node(node)) {
        auto while_node = std::dynamic_pointer_cast<WhileNode>(node);
        while_node->populate_pkb_parent(write_facade);
    }

    for (const auto& child : node->get_children()) {
        traverse(child, {});
    }

    return node;
}
} // namespace sp