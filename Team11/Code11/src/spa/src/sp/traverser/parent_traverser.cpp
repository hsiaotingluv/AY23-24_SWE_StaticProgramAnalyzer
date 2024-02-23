#include "sp/traverser/parent_traverser.hpp"
#include "common/ast/mixin/mixin_type_checker.hpp"

namespace sp {

auto ParentTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>&)
    -> std::shared_ptr<AstNode> {

    if (MixinTypeChecker::is_parent_mixin_node(node)) {
        auto parent_mixin = std::dynamic_pointer_cast<ParentMixin>(node);
        parent_mixin->populate_pkb_parent(write_facade);
    }

    for (const auto& child : node->get_children()) {
        traverse(child, {});
    }

    return node;
}
} // namespace sp