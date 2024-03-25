#pragma once

#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"

namespace sp {

// Only if and while are parents

class ParentTraverser : public Traverser {
    std::shared_ptr<pkb::WriteFacade> write_facade;

  public:
    explicit ParentTraverser(std::shared_ptr<pkb::WriteFacade> write_facade) : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp