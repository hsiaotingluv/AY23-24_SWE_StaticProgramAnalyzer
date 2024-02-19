#pragma once

#include "common/ast/statement_ast.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"

namespace sp {

class AssignmentPopulatorTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;

    static auto is_assignment_node(const std::shared_ptr<AstNode>& node) -> bool;

    auto generate_postfix_expr(const std::shared_ptr<AstNode>& node) -> std::string;
    static auto is_binop_node(const std::shared_ptr<AstNode>& node) -> bool;

  public:
    explicit AssignmentPopulatorTraverser(std::shared_ptr<WriteFacade> write_facade)
        : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>&) -> std::shared_ptr<AstNode> override;
};
} // namespace sp