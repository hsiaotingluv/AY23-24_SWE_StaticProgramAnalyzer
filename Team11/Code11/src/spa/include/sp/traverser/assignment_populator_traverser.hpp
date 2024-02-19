#pragma once

#include "sp/traverser/traverser.hpp"

namespace sp {

class AssignmentPopulatorTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;

    auto generate_postfix_expr(const std::shared_ptr<AstNode>& node) -> std::string;

  public:
    explicit AssignmentPopulatorTraverser(std::shared_ptr<WriteFacade> write_facade)
        : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>&) -> std::shared_ptr<AstNode> override;
};
} // namespace sp