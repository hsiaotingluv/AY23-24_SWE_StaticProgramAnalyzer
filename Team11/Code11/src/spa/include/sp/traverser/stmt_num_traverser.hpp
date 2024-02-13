#pragma once

#include "PKB/Facades/WriteFacade.h"
#include "common/ast/statement_ast.hpp"
#include "sp/traverser/traverser.hpp"

namespace sp {

class StmtNumTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;
    static auto is_stmt_node(const std::shared_ptr<AstNode>& node) -> bool;
    auto set_stmt_num(const std::shared_ptr<AstNode>& node, uint32_t stmt_num) -> uint32_t;

    static auto is_assign_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto is_call_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto is_if_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto is_while_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto is_read_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto is_print_node(const std::shared_ptr<AstNode>& node) -> bool;
    auto add_statement_pkb(uint32_t stmt_num, const std::shared_ptr<StatementNode>& stmt_node) -> void;

  public:
    explicit StmtNumTraverser(std::shared_ptr<WriteFacade> write_facade) : write_facade(std::move(write_facade)){};
    auto traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> override;
};
} // namespace sp