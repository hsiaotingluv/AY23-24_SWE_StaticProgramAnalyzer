#include "sp/validator/semantic_validator.hpp"
#include "common/ast/ast.hpp"
#include "common/ast/procedure_ast.hpp"
#include "common/ast/statement_ast.hpp"
#include "sp/validator/exception/semantic_error.hpp"
#include <queue>
#include <vector>

namespace sp {

auto SemanticValidator::validate_get_traversal_order(const std::shared_ptr<AstNode>& program_node)
    -> std::vector<std::string> {

    // Populate proc name, check for duplicate proc
    for (const auto& proc : program_node->get_children()) {
        auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(proc);
        if (proc_node == nullptr) {
            throw SemanticError("Unable to get proc node from program node");
        }

        if (proc_name_set.find(proc_node->proc_name) != proc_name_set.end()) {
            throw SemanticError("Duplicate Function Name");
        } else {
            proc_name_set.insert(proc_node->proc_name);
        }
    }

    // Populate call graph, cannot call non-existing procedure
    for (const auto& proc_name : proc_name_set) {
        dependency_graph.insert(std::make_pair(proc_name, std::unordered_set<std::string>()));
        indegree_map.insert(std::make_pair(proc_name, 0));
    }

    for (const auto& proc : program_node->get_children()) {
        auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(proc);
        build_graph(proc_node, proc_node->proc_name);
    }

    // No recursive and cyclic call, use Topological sort to detect this
    auto topo_sort = topological_sort();

    return topo_sort;
}

auto SemanticValidator::build_graph(const std::shared_ptr<AstNode>& node, const std::string& caller_name) -> void {
    auto call_node_opt = std::dynamic_pointer_cast<CallNode>(node);
    if (call_node_opt != nullptr) {
        auto callee = call_node_opt->proc_name;
        if (proc_name_set.find(callee) == proc_name_set.end()) {
            throw SemanticError(caller_name + " trying to call non-existing callee " + callee);
        }

        auto& callee_node = dependency_graph.at(callee);
        if (callee_node.find(caller_name) == callee_node.end()) {
            callee_node.insert(caller_name);
            indegree_map.at(caller_name)++;
        }

    } else {
        for (const auto& child : node->get_children()) {
            build_graph(child, caller_name);
        }
    }
}

auto SemanticValidator::topological_sort() -> std::vector<std::string> {
    auto q = std::queue<std::string>();
    for (const auto& entry : indegree_map) {
        if (entry.second == 0) {
            q.push(entry.first);
        }
    }

    auto result = std::vector<std::string>();

    while (!q.empty()) {
        auto curr_node = q.front();
        q.pop();
        result.push_back(curr_node);
        for (const auto& neighbour : dependency_graph.at(curr_node)) {
            if (--indegree_map.at(neighbour) == 0) {
                q.push(neighbour);
            }
        }
    }

    if (result.size() != proc_name_set.size()) {
        throw SemanticError("There is a cycle in the call Graph, unable to generate topological sort");
    }

    return result;
}
} // namespace sp