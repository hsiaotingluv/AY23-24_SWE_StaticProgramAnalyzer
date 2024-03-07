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
    using StatementNumber = std::vector<int>;

  public:
    StatementNumber stmt_nums; // Public for Testing purposes

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
            os << cfg_node.stmt_nums[i];
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
    using OutNeighbours =
        std::pair<std::shared_ptr<CfgNode>, std::shared_ptr<CfgNode>>; // If and While CfgNode have 2 out-neighbours.
    using Graph =
        std::unordered_map<std::shared_ptr<CfgNode>, OutNeighbours>; // Adjacency List of CfgNodes -> OutNeighbours.
    static inline const OutNeighbours EMPTY_OUTNEIGHBOURS = std::make_pair(nullptr, nullptr);

  public:
    std::shared_ptr<CfgNode> current_node; // Used only to build the Cfg.
    Graph graph{};

    explicit Cfg() {
        current_node = std::make_shared<CfgNode>(); // Writing stmts in the constructor body, for readability.
        add_node_to_graph();
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
        graph.insert(std::make_pair(current_node, EMPTY_OUTNEIGHBOURS));
    }

    /**
     * @brief Add a new node to the graph and link it to the current node.
     */
    auto add_outneighbour_to_graph(std::shared_ptr<CfgNode> out_neighbour) -> void {
        if (graph.find(current_node) == graph.end()) {
            add_node_to_graph();
        }
        // Fill the out-neighbours of current_node.
        if (graph[current_node].first) {
            graph[current_node].second = out_neighbour;
        } else {
            graph[current_node].first = out_neighbour;
        }
    }

    /**
     * @brief Move current node to the next node (Need not be a outneighbour node).
     * @note This is only explicitly used for If CfgNode to build both branches of the If statement.
     */
    auto move_to(std::shared_ptr<CfgNode> next_node) -> void {
        current_node = next_node;
    };

    /**
     * @brief Add outneighbour node to the graph and move current node to the outneighbour node.
     * @note Default way to traverse the Cfg.
     */
    auto link_and_move_to(std::shared_ptr<CfgNode> next_node) -> void {
        add_outneighbour_to_graph(next_node);
        move_to(next_node);
        add_node_to_graph();
    };

    /**
     * @brief Construct a string representation of the Cfg. e.g. "Node(4, 5, 6) -> OutNeighbours(Node(7, 8, 9), Node(10,
     * 11, 12))"
     */
    friend auto operator<<(std::ostream& os, const Cfg& cfg) -> std::ostream& {
        auto graph = cfg.graph;
        for (const auto& [node, outneighbours] : graph) {
            os << *node;
            os << " -> OutNeighbours(";
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