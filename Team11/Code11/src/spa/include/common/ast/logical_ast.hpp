#pragma once

#include "common/ast/ast.hpp"

namespace sp {
class LogicalNode : public AstNode {
    using AstNode::AstNode;
};

class LogicalBinaryNode : public LogicalNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit LogicalBinaryNode(sp::NodeType T, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalNode(T), left(std::move(left)), right(std::move(right)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {left, right};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << *left << ", " << *right << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        auto start_xml = "<" + get_node_name() + ">";
        auto left_xml = left->to_xml();
        auto right_xml = right->to_xml();
        auto end_xml = "</" + get_node_name() + ">";

        return start_xml + left_xml + right_xml + end_xml;
    }
};

class LogicalAndNode : public LogicalBinaryNode {
  public:
    explicit LogicalAndNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalBinaryNode(sp::NodeType::LAnd, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LogicalAndNode";
    }
};

class LogicalOrNode : public LogicalBinaryNode {
  public:
    explicit LogicalOrNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalBinaryNode(sp::NodeType::LOr, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LogicalOrNode";
    }
};

class LogicalNotNode : public LogicalNode {
  public:
    std::shared_ptr<AstNode> cond_expr;

    explicit LogicalNotNode(std::shared_ptr<AstNode>& cond_expr)
        : LogicalNode(sp::NodeType::LNot), cond_expr(cond_expr) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {cond_expr};
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LogicalNotNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << *cond_expr << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        auto start_xml = "<" + get_node_name() + ">";
        auto cond_expr_xml = cond_expr->to_xml();
        auto end_xml = "</" + get_node_name() + ">";

        return start_xml + cond_expr_xml + end_xml;
    }
};
} // namespace sp