#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

namespace sp {

using StatementNumbers = std::vector<int>; // Need to maintain order

/**
 * @brief CfgNode represents a Control Flow Graph's Node.
 * @note If or While CfgNode must have 1 statement number. Otherwise, other CfgNode can have multiple consecutive
 * statement numbers.
 */
class CfgNode {
  private:
    StatementNumbers stmt_nums{};

  public:
    /**
     * @brief Get all statement numbers in the CfgNode.
     */
    [[nodiscard]] auto get() const -> StatementNumbers;

    /**
     * @brief Add a statement number.
     */
    auto add(int stmt_num) -> int;

    /**
     * @brief Check if the CfgNode has no statement numbers.
     */
    [[nodiscard]] auto empty() const -> bool;

    /**
     * @brief Construct a string representation of the CfgNode. e.g. "Node(4, 5, 6)"
     */
    friend auto operator<<(std::ostream& os, const CfgNode& cfg_node) -> std::ostream&;
};

using OutNeighbours = std::pair<std::shared_ptr<CfgNode>, std::shared_ptr<CfgNode>>;
using Graph =
    std::unordered_map<std::shared_ptr<CfgNode>, OutNeighbours>; // Adjacency List of CfgNodes -> OutNeighbours.

/**
 * @brief ProcedureCfg represents a Control Flow Graph, a directed graph of CfgNodes.
 * @note There is one ProcedureCfg per Procedure.
 */
class ProcedureCfg {
    /**
     * For normal Statements: OutNeighbours.first is the next node. OutNeighbours.second is nullptr.
     * For If Statements: OutNeighbours.first is the then-block node. OutNeighbours.second is the else-block node.
     * For While Statements: OutNeighbours.first is the loop-block node. OutNeighbours.second is next node after the
     * While block.
     */
    static inline const OutNeighbours EMPTY_OUTNEIGHBOURS = std::make_pair(nullptr, nullptr);

  private:
    std::shared_ptr<CfgNode> start_node;
    std::shared_ptr<CfgNode> current_node; // Used only to build the ProcedureCfg.
    Graph graph{};

  public:
    explicit ProcedureCfg() : start_node(std::make_shared<CfgNode>()), current_node(start_node) {
        add_node_to_graph();
    }

    /**
     * @brief Get the start node.
     */
    auto get_start_node() const -> std::shared_ptr<CfgNode>;

    /**
     * @brief Get the current node.
     */
    auto get_current_node() const -> std::shared_ptr<CfgNode>;

    /**
     * @brief Get the associated OutNeighbours
     */
    auto get_outneighbours(const std::shared_ptr<CfgNode>& node) const -> OutNeighbours;

    /**
     * @brief Get the graph.
     */
    auto get_graph() const -> Graph;

    /**
     * @brief Check if the current node is empty.
     */
    auto is_current_node_empty() const -> bool;

    /**
     * @brief Add a statement number to the current node.
     */
    auto add_stmt_to_node(int stmt_num) -> int;

    /**
     * @brief Add a new node to the graph, with empty out-neighbours.
     */
    auto add_node_to_graph() -> std::shared_ptr<CfgNode>;

    /**
     * @brief Add a new node to the graph and link it to the current node.
     */
    auto add_outneighbour_to_graph(const std::shared_ptr<CfgNode>& outneighbour) -> std::shared_ptr<CfgNode>;

    /**
     * @brief Move current node to the next node (Need not be a outneighbour node).
     * @note This is only explicitly used for If CfgNode to build both branches of the If statement.
     */
    auto move_to(const std::shared_ptr<CfgNode>& node) -> std::shared_ptr<CfgNode>;

    /**
     * @brief Add outneighbour node to the graph and move current node to the outneighbour node.
     * @note Default way to traverse the ProcedureCfg.
     */
    auto link_and_move_to(const std::shared_ptr<CfgNode>& node) -> std::shared_ptr<CfgNode>;

    /**
     * @brief Construct a string representation of the ProcedureCfg. e.g. "Node(4, 5, 6) -> OutNeighbours(Node(7, 8, 9),
     * Node(10, 11, 12))"
     */
    friend auto operator<<(std::ostream& os, const ProcedureCfg& cfg) -> std::ostream&;
};
} // namespace sp