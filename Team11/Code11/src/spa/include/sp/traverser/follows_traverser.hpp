#pragma once

#include "common/ast/statement_ast.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"

namespace sp {

class FollowsTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;

  public:
    explicit FollowsTraverser(std::shared_ptr<WriteFacade> write_facade) : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp