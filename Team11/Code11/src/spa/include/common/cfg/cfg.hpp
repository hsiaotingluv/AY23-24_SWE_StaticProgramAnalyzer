#pragma once

#include <memory>
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
    auto empty() -> bool {
        return stmt_nums.size() == 0;
    };

    /**
     * @brief Construct a string representation of the CfgNode. e.g. "Node(4, 5, 6)"
     */
    auto to_string() -> std::string {
        std::string str_rep = "Node(";
        for (const auto& stmt_num : stmt_nums) {
            str_rep += std::to_string(stmt_num) + ", ";
        }
        str_rep = str_rep.substr(0, str_rep.size() - 2); // Remove final ", "
        str_rep += ")";
        return str_rep;
    };
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
  public:
    std::shared_ptr<CfgNode> current_node;
    Graph graph{};

    explicit Cfg() : current_node(std::make_shared<CfgNode>()), graph() {
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
        graph.insert(std::make_pair(current_node, std::make_pair(nullptr, nullptr)));
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
     * @note This is only used for If CfgNode to build both branches of the If statement.
     */
    auto next(std::shared_ptr<CfgNode> next_node) -> void {
        current_node = next_node;
    };

    /**
     * @brief Add outneighbour node to the graph and move current node to the outneighbour node.
     * @note Default way to traverse the Cfg.
     */
    auto link_and_next(std::shared_ptr<CfgNode> next_node) -> void {
        add_outneighbour_to_graph(next_node);
        next(next_node);
        add_node_to_graph();
    };

    /**
     * @brief Construct a string representation of the Cfg. e.g. "Node(4, 5, 6) -> OutNeighbours(Node(7, 8, 9), Node(10,
     * 11, 12))"
     */
    auto to_string() -> std::string {
        std::string str_rep = "";
        for (const auto& [node, outneighbours] : graph) {
            std::string edge = node->to_string() + " -> OutNeighbours(";
            if (outneighbours.first) {
                edge += outneighbours.first->to_string();
            }
            if (outneighbours.second) {
                edge += ", " + outneighbours.second->to_string();
            }
            edge += ")\n";
            str_rep += edge;
        }
        return str_rep;
    };
};
} // namespace sp