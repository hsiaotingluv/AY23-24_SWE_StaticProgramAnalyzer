#include "common/cfg/cfg.hpp"
#include <iostream>
#include <utility>

namespace sp {
auto CfgNode::get() const -> StatementNumbers {
    return stmt_nums;
}

auto CfgNode::add(int stmt_num) -> int {
    stmt_nums.push_back(stmt_num);
    return stmt_num;
}

auto CfgNode::empty() const -> bool {
    return stmt_nums.empty();
}

auto operator<<(std::ostream& os, const CfgNode& cfg_node) -> std::ostream& {
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

auto ProcedureCfg::get_start_node() const -> std::shared_ptr<CfgNode> {
    return start_node;
}

auto ProcedureCfg::get_current_node() const -> std::shared_ptr<CfgNode> {
    return current_node;
}

auto ProcedureCfg::get_outneighbours(const std::shared_ptr<CfgNode>& node) const -> OutNeighbours {
    if (graph.find(node) == graph.end()) {
        return EMPTY_OUTNEIGHBOURS;
    } else {
        return graph.at(node);
    }
}

auto ProcedureCfg::get_graph() const -> Graph {
    return graph;
}

auto ProcedureCfg::is_current_node_empty() const -> bool {
    return current_node->empty();
}

auto ProcedureCfg::add_stmt_to_node(int stmt_num) -> int {
    current_node->add(stmt_num);
    return stmt_num;
}

auto ProcedureCfg::add_node_to_graph() -> std::shared_ptr<CfgNode> {
    auto edge = std::make_pair(current_node, EMPTY_OUTNEIGHBOURS);
    graph.insert(edge);
    return current_node;
}

auto ProcedureCfg::add_outneighbour_to_graph(const std::shared_ptr<CfgNode>& outneighbour) -> std::shared_ptr<CfgNode> {
    // Fill the out-neighbours of current_node.
    if (graph.at(current_node).first) {
        graph.at(current_node).second = outneighbour;
    } else {
        graph.at(current_node).first = outneighbour;
    }
    return outneighbour;
}

auto ProcedureCfg::move_to(const std::shared_ptr<CfgNode>& node) -> std::shared_ptr<CfgNode> {
    current_node = node;
    return node;
}

auto ProcedureCfg::link_and_move_to(const std::shared_ptr<CfgNode>& node) -> std::shared_ptr<CfgNode> {
    add_outneighbour_to_graph(node);
    move_to(node);
    add_node_to_graph();
    return node;
}

auto operator<<(std::ostream& os, const ProcedureCfg& cfg) -> std::ostream& {
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
} // namespace sp