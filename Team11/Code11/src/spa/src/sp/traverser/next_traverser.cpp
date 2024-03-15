#include "sp/traverser/next_traverser.hpp"

namespace sp {

/**
 * @brief Extract the Next Relationships between statements within a CFG node.
 */
auto NextTraverser::traverse_node(const std::shared_ptr<CfgNode>& node) -> void {
    auto stmt_nums = node->get();
    for (size_t i = 1; i < stmt_nums.size(); i++) {
        auto prev_stmt_num = stmt_nums.at(i - 1);
        auto curr_stmt_num = stmt_nums.at(i);
        write_facade->add_next(std::to_string(prev_stmt_num), std::to_string(curr_stmt_num));
    }
}

/**
 * @brief Extract the Next Relationships between the last statement of a node and the first statement of its
 * outneighbour.
 */
auto NextTraverser::traverse_edge(const std::shared_ptr<CfgNode>& node, const std::shared_ptr<CfgNode>& outneighbour)
    -> void {
    auto prev_node_stmt_nums = node->get();
    auto prev_node_final_stmt_num = std::to_string(prev_node_stmt_nums.back());

    auto outneigh_stmt_nums = outneighbour->get();
    auto outneigh_first_stmt_num = std::to_string(outneigh_stmt_nums.front());

    write_facade->add_next(prev_node_final_stmt_num, outneigh_first_stmt_num);
}

/**
 * @brief Extract the Next Relationships between the last statement of a node and the first statements of its
 * outneighbours.
 */
auto NextTraverser::traverse_edges(const std::shared_ptr<CfgNode>& node, const OutNeighbours& outneighbours) -> void {
    auto is_first_outneighbour_non_empty = outneighbours.first && !(outneighbours.first->empty());
    auto is_second_outneighbour_non_empty = outneighbours.second && !(outneighbours.second->empty());

    if (is_first_outneighbour_non_empty) {
        traverse_edge(node, outneighbours.first);
    }
    if (is_second_outneighbour_non_empty) {
        traverse_edge(node, outneighbours.second);
    }
}

/**
 * @brief Extract the Next Relationships within a procedure.
 */
auto NextTraverser::traverse_procedure(const std::shared_ptr<Cfg>& cfg) -> void {
    auto graph = cfg->get_graph();
    for (const auto& [node, outneighbours] : graph) {
        traverse_node(node);

        auto is_node_non_empty = !(node->empty());
        auto is_first_neighb_non_empty = outneighbours.first && !(outneighbours.first->empty());
        auto is_second_neighb_non_empty = outneighbours.second && !(outneighbours.second->empty());

        if (is_node_non_empty && (is_first_neighb_non_empty || is_second_neighb_non_empty)) {
            traverse_edges(node, outneighbours);
        } // Else, there is no edge to link.
    }
}

/**
 * @brief Extract the Next Relationships for the Program.
 * @note ProcMap is an alias for std::unordered_map<std::string, std::shared_ptr<Cfg>>
 */
auto NextTraverser::traverse(const ProcMap& cfgs) -> void {
    for (const auto& [proc_name, cfg] : cfgs) {
        traverse_procedure(cfg);
    }
}

} // namespace sp
