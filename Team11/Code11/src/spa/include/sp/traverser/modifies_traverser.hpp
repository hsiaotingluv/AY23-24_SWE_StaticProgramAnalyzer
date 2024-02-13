#pragma once

#include "PKB/Facades/WriteFacade.h"
#include "sp/traverser/traverser.hpp"

namespace sp {

class ModifiesTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;

    static auto is_read_node(const std::shared_ptr<AstNode>& node) -> bool;

    static auto is_assignment_node(const std::shared_ptr<AstNode>& node) -> bool;

  public:
    explicit ModifiesTraverser(std::shared_ptr<WriteFacade> write_facade) : write_facade(std::move(write_facade)){};
    auto traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> override;
};
} // namespace sp