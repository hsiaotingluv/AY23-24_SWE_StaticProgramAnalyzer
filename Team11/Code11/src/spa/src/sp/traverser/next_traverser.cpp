#include "sp/traverser/next_traverser.hpp"

namespace sp {

/**
 * @brief Extract the Next Relationships between statements within a CFG node.
 */
auto NextTraverser::traverse_node(std::shared_ptr<CfgNode> node) -> void {
    auto stmt_nums = node->get();
    for (size_t i = 1; i < stmt_nums.size(); i++) {
        auto prev_stmt_num = stmt_nums.at(i - 1);
        auto curr_stmt_num = stmt_nums.at(i);
        write_facade->add_next(std::to_string(prev_stmt_num), std::to_string(curr_stmt_num));
    }
}

/**
 * @brief Extract the Next Relationships between the last statement of a node and the first statements of its
 * outneighbours.
 */
auto NextTraverser::traverse_edge(std::shared_ptr<CfgNode> node, OutNeighbours outneighbours) -> void {
    auto prev_node_stmt_nums = node->get();
    auto prev_node_final_stmt_num = prev_node_stmt_nums.back();

    if (outneighbours.first && !(outneighbours.first->empty())) {
        auto first_neighb_stmt_nums = outneighbours.first->get();
        auto first_neighb_first_stmt_num = first_neighb_stmt_nums.front();
        write_facade->add_next(std::to_string(prev_node_final_stmt_num), std::to_string(first_neighb_first_stmt_num));
    }
    if (outneighbours.second && !(outneighbours.second->empty())) {
        auto second_neighb_stmt_nums = outneighbours.second->get();
        auto second_neighb_first_stmt_num = second_neighb_stmt_nums.front();
        write_facade->add_next(std::to_string(prev_node_final_stmt_num), std::to_string(second_neighb_first_stmt_num));
    }
}

/**
 * @brief Extract the Next Relationships within a procedure.
 */
auto NextTraverser::traverse_procedure(std::shared_ptr<Cfg> cfg) -> void {
    auto graph = cfg->get_graph();
    for (const auto& [node, outneighbours] : graph) {
        traverse_node(node);

        bool is_node_non_empty = !(node->empty());
        bool is_first_neighb_non_empty = outneighbours.first && !(outneighbours.first->empty());
        bool is_second_neighb_non_empty = outneighbours.second && !(outneighbours.second->empty());

        if (is_node_non_empty && (is_first_neighb_non_empty || is_second_neighb_non_empty)) {
            traverse_edge(node, outneighbours);
        } // Else, there is no edge to link.
    }
}

/**
 * @brief Extract the Next Relationships for the Program.
 * @note ProcMap is an alias for std::unordered_map<std::string, std::shared_ptr<Cfg>>
 */
auto NextTraverser::traverse(ProcMap cfgs) -> void {
    for (const auto& [proc_name, cfg] : cfgs) {
        std::cout << *cfg << std::endl;
        traverse_procedure(cfg);
    }
}

} // namespace sp
