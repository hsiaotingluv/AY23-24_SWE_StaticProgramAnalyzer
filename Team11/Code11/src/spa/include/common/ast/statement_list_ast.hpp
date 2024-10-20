#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/mixin/cfg_mixin.hpp"

namespace sp {

/**
 * @brief StatementListNode is an AST Node represents a StatementList.
 */
class StatementListNode : public sp::AstNode, public sp::CfgMixin {
  public:
    std::vector<std::shared_ptr<AstNode>> statements;

    explicit StatementListNode(std::vector<std::shared_ptr<AstNode>>& statements)
        : AstNode(sp::NodeType::StmtList), statements(std::move(statements)) {
    }

    // AstNode methods.
    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;

    // CfgMixin methods.
    auto build_cfg(std::shared_ptr<ProcedureCfg> cfg) -> void override;
};
} // namespace sp
