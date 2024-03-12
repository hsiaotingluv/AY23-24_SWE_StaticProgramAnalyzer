#pragma once

#include "common/ast/ast.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace sp {

class SemanticValidator {
  public:
    using CallGraph = std::unordered_map<std::string, std::unordered_set<std::string>>;

  private:
    using CallSet = std::unordered_set<std::string>;
    using IndegreeMap = std::unordered_map<std::string, int>;

    CallGraph dependency_graph{}; // from callee -> callers
    CallSet proc_name_set{};
    IndegreeMap indegree_map{};

    auto build_graph(const std::shared_ptr<AstNode>& node, const std::string& caller_name) -> void;
    auto topological_sort() -> std::vector<std::string>;

  public:
    explicit SemanticValidator() = default;
    auto validate_get_traversal_order(const std::shared_ptr<AstNode>& program_node) -> std::vector<std::string>;

    /**
     * @brief The graph is populated after AST validation
     */
    auto get_call_graph() const -> const CallGraph& {
        return dependency_graph;
    };
};
} // namespace sp
