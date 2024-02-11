#pragma once

#include "PKB/Facades/WriteFacade.h"
#include "sp/traverser/traverser.hpp"

namespace sp {

class ProcedurePopulatorTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;

    static bool is_proc_node(const std::shared_ptr<AstNode>& node);

  public:
    explicit ProcedurePopulatorTraverser(std::shared_ptr<WriteFacade> write_facade)
        : write_facade(std::move(write_facade)){};

    auto traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> override;
};
} // namespace sp