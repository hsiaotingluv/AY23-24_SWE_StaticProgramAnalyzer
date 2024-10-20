#pragma once

#include "common/ast/ast.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"

namespace sp {

class DesignEntitiesPopulatorTraverser : public Traverser {
    std::shared_ptr<pkb::WriteFacade> write_facade;

  public:
    explicit DesignEntitiesPopulatorTraverser(std::shared_ptr<pkb::WriteFacade> write_facade)
        : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>&) -> std::shared_ptr<AstNode> override;
};
} // namespace sp