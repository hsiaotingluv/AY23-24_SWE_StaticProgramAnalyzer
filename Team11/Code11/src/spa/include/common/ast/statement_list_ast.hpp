#pragma once

#include "common/ast/ast.hpp"

namespace sp {
class StatementListNode : public sp::AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> statements;

    explicit StatementListNode(std::vector<std::shared_ptr<AstNode>>& statements)
        : statements(std::move(statements)), AstNode(sp::NodeType::StmtList) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return statements;
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "StatementListNode(";
        for (const auto& stmt : statements) {
            ss << *stmt << ", ";
        }
        ss << ")";
        return ss;
    }
};
} // namespace sp
