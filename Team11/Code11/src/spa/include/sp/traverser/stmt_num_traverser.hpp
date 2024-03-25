#pragma once

#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"

namespace sp {

class StmtNumTraverser : public Traverser {
    std::shared_ptr<pkb::WriteFacade> write_facade;
    auto set_stmt_num(const std::shared_ptr<AstNode>& node, uint32_t stmt_num) -> uint32_t;

  public:
    explicit StmtNumTraverser(std::shared_ptr<pkb::WriteFacade> write_facade) : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp