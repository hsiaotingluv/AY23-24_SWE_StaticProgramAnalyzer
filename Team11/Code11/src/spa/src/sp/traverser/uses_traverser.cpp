#include "sp/traverser/uses_traverser.hpp"

// TODO: File is getting big. To split into individual uses_by_statements.cpp

namespace sp {

auto UsesTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
    -> std::shared_ptr<AstNode> {
    for (const auto& node : node->get_children()) {
        const auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        proc_map.insert(std::make_pair(proc_node->proc_name, proc_node));
    }

    // Traversing using topo sort order allow each traversal to check what's the modifies in each call stmt
    auto uses_map = std::make_shared<UsesMap>();
    std::for_each(proc_topo_sort.begin(), proc_topo_sort.end(), [&](const auto& proc_order) {
        auto proc_node = proc_map.at(proc_order);
        proc_node->populate_pkb_uses(write_facade, uses_map);
    });

    return node;
}
} // namespace sp