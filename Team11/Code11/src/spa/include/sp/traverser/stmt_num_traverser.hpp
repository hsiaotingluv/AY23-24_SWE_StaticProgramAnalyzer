#pragma once

#include "sp/traverser/traverser.hpp"

namespace sp {

class StmtNumTraverser : public Traverser {
  private:
    static auto is_stmt_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto set_stmt_num(const std::shared_ptr<AstNode>& node, uint32_t stmt_num) -> uint32_t;

  public:
    auto traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> override;
};
} // namespace sp