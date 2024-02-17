#pragma once

#include "common/ast/ast.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace sp {

class SemanticValidator {
    using CallGraph = std::unordered_map<std::string, std::unordered_set<std::string>>;
    using CallSet = std::unordered_set<std::string>;
    using IndegreeMap = std::unordered_map<std::string, int>;

    CallGraph dependency_graph{};
    CallSet proc_name_set{};
    IndegreeMap indegree_map{};

    auto build_graph(const std::shared_ptr<AstNode>& node, const std::string& caller_name) -> void;
    auto topological_sort() -> std::vector<std::string>;

  public:
    explicit SemanticValidator() = default;
    auto validate_get_traversal_order(const std::shared_ptr<AstNode>& program_node) -> std::vector<std::string>;
};
} // namespace sp
