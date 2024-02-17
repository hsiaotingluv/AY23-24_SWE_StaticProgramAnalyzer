#pragma once

#include "common/ast/ast.hpp"

namespace sp {
class StatementListNode : public sp::AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> statements;

    explicit StatementListNode(std::vector<std::shared_ptr<AstNode>>& statements)
        : AstNode(sp::NodeType::StmtList), statements(std::move(statements)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return statements;
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "StatementListNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(";
        for (const auto& stmt : statements) {
            ss << *stmt << ", ";
        }
        ss << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        auto xml = "<" + get_node_name() + ">";
        for (const auto& stmt : statements) {
            xml += stmt->to_xml();
        }
        xml += "</" + get_node_name() + ">";
        return xml;
    }
};
} // namespace sp
