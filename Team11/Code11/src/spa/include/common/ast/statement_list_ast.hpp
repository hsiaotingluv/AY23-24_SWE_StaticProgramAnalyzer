#pragma once

#include "common/ast/ast.hpp"

namespace sp {
class StatementListNode : public sp::AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> statements;

    explicit StatementListNode(std::vector<std::shared_ptr<AstNode>>& statements)
        : AstNode(sp::NodeType::StmtList), statements(std::move(statements)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
};
} // namespace sp
