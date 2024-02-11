#pragma once

#include "sp/annotator/annotator.hpp"

namespace sp {

class StmtNumAnnotator : public Annotator {
  private:
    static auto is_stmt_node(std::shared_ptr<AstNode> node) -> bool;
    static auto set_stmt_num(std::shared_ptr<AstNode> node, uint32_t stmt_num) -> uint32_t;

  public:
    auto annotate(std::shared_ptr<AstNode> ast) -> std::shared_ptr<AstNode> override;
};
} // namespace sp