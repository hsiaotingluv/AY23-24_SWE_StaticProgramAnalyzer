#include "common/ast/binary_node_ast.hpp"
#include "common/ast/factor_ast.hpp"
#include "sp/traverser/design_entites_populator_traverser.hpp"
#include <memory>

namespace sp {
auto DesignEntitiesPopulatorTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>&)
    -> std::shared_ptr<AstNode> {
    auto casted_node = std::dynamic_pointer_cast<DesignEntitiesMixin>(node);
    if (casted_node != nullptr) {
        casted_node->populate_pkb_entities(write_facade);
    }

    for (const auto& child : node->get_children()) {
        traverse(child, {});
    }

    return node;
}
} // namespace sp
