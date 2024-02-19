#include "sp/traverser/modifies_traverser.hpp"

namespace sp {
auto ModifiesTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
    -> std::shared_ptr<AstNode> {
    for (const auto& node : node->get_children()) {
        const auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        proc_map.insert(std::make_pair(proc_node->proc_name, proc_node));
    }

    // Traversing using topo sort order allow each traversal to check what's the modifies in each call stmt
    auto modify_map = std::make_shared<ModifyMap>();

    for (const auto& proc_order : proc_topo_sort) {
        auto proc_node = proc_map.at(proc_order);
        proc_node->populate_pkb_modifies(write_facade, modify_map);
    }

    return node;
}

} // namespace sp
