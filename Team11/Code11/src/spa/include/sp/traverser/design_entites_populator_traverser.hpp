#pragma once

#include "sp/traverser/traverser.hpp"

#include <memory>

namespace sp {

class DesignEntitiesPopulatorTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;

    auto add_statement_pkb(uint32_t stmt_num, const std::shared_ptr<StatementNode>& stmt_node) -> void;

  public:
    explicit DesignEntitiesPopulatorTraverser(std::shared_ptr<WriteFacade> write_facade)
        : write_facade(std::move(write_facade)){};

    auto traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>&) -> std::shared_ptr<AstNode> override;
};
} // namespace sp