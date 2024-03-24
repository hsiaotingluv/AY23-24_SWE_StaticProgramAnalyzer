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
auto NextTraverser::traverse_edge(const std::shared_ptr<CfgNode>& node, const std::shared_ptr<CfgNode>& outneighbour,
                                  const Graph& proc_cfg_graph) -> void {
    if (outneighbour == nullptr) {
        return;
    }

    auto prev_node_stmt_nums = node->get();
    auto prev_node_final_stmt_num = std::to_string(prev_node_stmt_nums.back());

    auto is_neighbour_dummy = outneighbour->empty();
    if (is_neighbour_dummy) {
        auto dummy_node_next = proc_cfg_graph.find(outneighbour);
        if (dummy_node_next != proc_cfg_graph.end()) {
            auto dummy_node_outneighbours = dummy_node_next->second;

            // Dummy neighbour will only have 1 outneighbour
            auto dummy_neighbour = dummy_node_outneighbours.first;
            traverse_edge(node, dummy_neighbour, proc_cfg_graph);
        }
    } else {
        auto outneighbour_stmt_nums = outneighbour->get();
        auto outneighbour_first_stmt_num = std::to_string(outneighbour_stmt_nums.front());

        write_facade->add_next(prev_node_final_stmt_num, outneighbour_first_stmt_num);
    }
}

/**
 * @brief Extract the Next Relationships between the last statement of a node and the first statements of its
 * outneighbours.
 */
auto NextTraverser::traverse_edges(const std::shared_ptr<CfgNode>& node, const OutNeighbours& outneighbours,
                                   const Graph& proc_cfg_graph) -> void {
    traverse_edge(node, outneighbours.first, proc_cfg_graph);
    traverse_edge(node, outneighbours.second, proc_cfg_graph);
}

/**
 * @brief Extract the Next Relationships within a procedure.
 */
auto NextTraverser::traverse_procedure(const std::shared_ptr<ProcedureCfg>& cfg) -> void {
    auto graph = cfg->get_graph();
    for (const auto& [node, outneighbours] : graph) {
        traverse_node(node);

        auto is_node_non_empty = !(node->empty());
        if (is_node_non_empty) {
            traverse_edges(node, outneighbours, graph);
        }
    }
}

/**
 * @brief Extract the Next Relationships for the Program.
 * @note ProcMap is an alias for std::unordered_map<std::string, std::shared_ptr<ProcedureCfg>>
 */
auto NextTraverser::traverse(const ProcMap& cfgs) -> void {
    for (const auto& [proc_name, cfg] : cfgs) {
        traverse_procedure(cfg);
    }
}

} // namespace sp
