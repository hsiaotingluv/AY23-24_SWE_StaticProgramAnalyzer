#pragma once

#include "common/ast/statement_ast.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"

namespace sp {

class StmtNumTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;
    auto set_stmt_num(const std::shared_ptr<AstNode>& node, uint32_t stmt_num) -> uint32_t;

    static auto is_stmt_node(const std::shared_ptr<AstNode>& node) -> bool;

  public:
    explicit StmtNumTraverser(std::shared_ptr<WriteFacade> write_facade) : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp