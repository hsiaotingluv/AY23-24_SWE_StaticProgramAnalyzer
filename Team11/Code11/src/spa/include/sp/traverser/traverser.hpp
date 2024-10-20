#pragma once

#include "common/ast/ast.hpp"

// A Traverser is a class that is used to traverse the AST with possible some side effects
// Traverser: AST -> AST
namespace sp {
class Traverser {
  public:
    virtual auto traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> = 0;
    virtual ~Traverser() = default;
};
} // namespace sp
