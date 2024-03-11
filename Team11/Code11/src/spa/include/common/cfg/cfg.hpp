#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace sp {
/**
 * @brief CfgNode represents a Control Flow Graph's Node.
 * @note If or While CfgNode must have 1 statement number. Otherwise, other CfgNode can have multiple consecutive
 * statement numbers.
 */
class CfgNode {

    using StatementNumbers = std::vector<int>; // Need to maintain order

  private:
    StatementNumbers stmt_nums{};

  public:
    /**
     * @brief Get all statement numbers in the CfgNode.
     */
    auto get() const -> StatementNumbers {
        return stmt_nums;
    };

    /**
     * @brief Construct a new Cfg Node object
     */
    auto add(int stmt_num) -> void {
        stmt_nums.push_back(stmt_num);
    };

    /**
     * @brief Check if the CfgNode has no statement numbers.
     */
    auto empty() const -> bool {
        return stmt_nums.size() == 0;
    };

    /**
     * @brief Construct a string representation of the CfgNode. e.g. "Node(4, 5, 6)"
     */
    friend auto operator<<(std::ostream& os, const CfgNode& cfg_node) -> std::ostream& {
        os << "Node(";
        for (size_t i = 0; i < cfg_node.stmt_nums.size(); i++) {
            if (i != 0) {
                os << ", ";
            }
            os << cfg_node.stmt_nums.at(i);
        }
        os << ")";
        return os;
    }
};

/**
 * @brief Cfg represents a Control Flow Graph, a directed graph of CfgNodes.
 * @note There is one Cfg per Procedure.
 */
class Cfg {
    /**
     * For normal Statements: OutNeighbours.first is the next node. OutNeighbours.second is nullptr.
     * For If Statements: OutNeighbours.first is the then-block node. OutNeighbours.second is the else-block node.
     * For While Statements: OutNeighbours.first is the loop-block node. OutNeighbours.second is next node after the
     * While block.
     */
    using OutNeighbours = std::pair<std::shared_ptr<CfgNode>, std::shared_ptr<CfgNode>>;
    using Graph =
        std::unordered_map<std::shared_ptr<CfgNode>, OutNeighbours>; // Adjacency List of CfgNodes -> OutNeighbours.
    static inline const OutNeighbours EMPTY_OUTNEIGHBOURS = std::make_pair(nullptr, nullptr);

  private:
    std::shared_ptr<CfgNode> start_node;
    std::shared_ptr<CfgNode> current_node; // Used only to build the Cfg.
    Graph graph{};

  public:
    explicit Cfg() {
        start_node = std::make_shared<CfgNode>();
        current_node = start_node;
        add_node_to_graph();
    }

    /**
     * @brief Get the start node.
     */
    auto get_start_node() const -> std::shared_ptr<CfgNode> {
        return start_node;
    };

    /**
     * @brief Get the current node.
     */
    auto get_current_node() const -> std::shared_ptr<CfgNode> {
        return current_node;
    };

    /**
     * @brief Get the associated OutNeighbours
     */
    auto get_outneighbours(std::shared_ptr<CfgNode> node) const -> OutNeighbours {
        if (graph.find(node) == graph.end()) {
            return EMPTY_OUTNEIGHBOURS;
        } else {
            return graph.at(node);
        }
    };

    /**
     * @brief Get the graph.
     */
    auto get_graph() const -> Graph {
        return graph;
    };

    /**
     * @brief Check if the current node is empty.
     */
    auto is_current_node_empty() const -> bool {
        return current_node->empty();
    }

    /**
     * @brief Add a statement number to the current node.
     */
    auto add_stmt_to_node(int stmt_num) -> void {
        current_node->add(stmt_num);
    };

    /**
     * @brief Add a new node to the graph, with empty out-neighbours.
     */
    auto add_node_to_graph() -> void {
        auto edge = std::make_pair(current_node, EMPTY_OUTNEIGHBOURS);
        graph.insert(edge);
    }

    /**
     * @brief Add a new node to the graph and link it to the current node.
     */
    auto add_outneighbour_to_graph(std::shared_ptr<CfgNode> outneighbour) -> void {
        // Fill the out-neighbours of current_node.
        if (graph.at(current_node).first) {
            graph.at(current_node).second = outneighbour;
        } else {
            graph.at(current_node).first = outneighbour;
        }
    }

    /**
     * @brief Move current node to the next node (Need not be a outneighbour node).
     * @note This is only explicitly used for If CfgNode to build both branches of the If statement.
     */
    auto move_to(std::shared_ptr<CfgNode> node) -> void {
        current_node = node;
    };

    /**
     * @brief Add outneighbour node to the graph and move current node to the outneighbour node.
     * @note Default way to traverse the Cfg.
     */
    auto link_and_move_to(std::shared_ptr<CfgNode> node) -> void {
        add_outneighbour_to_graph(node);
        move_to(node);
        add_node_to_graph();
    };

    /**
     * @brief Construct a string representation of the Cfg. e.g. "Node(4, 5, 6) -> OutNeighbours(Node(7, 8, 9), Node(10,
     * 11, 12))"
     */
    friend auto operator<<(std::ostream& os, const Cfg& cfg) -> std::ostream& {
        auto graph = cfg.graph;
        for (const auto& [node, outneighbours] : graph) {
            os << *node << " -> OutNeighbours(";
            if (outneighbours.first) {
                os << *outneighbours.first;
            }
            if (outneighbours.second) {
                os << ", " << *outneighbours.second;
            }
            os << ")\n";
        }
        return os;
    }
};
} // namespace sp