#pragma once

#include "common/ast/ast.hpp"

namespace sp {
class LogicalNode : public sp::AstNode {
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
};

class LogicalAndNode : public LogicalBinaryNode {
  public:
    explicit LogicalAndNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalBinaryNode(sp::NodeType::LAnd, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LogicalAndNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class LogicalOrNode : public LogicalBinaryNode {
  public:
    explicit LogicalOrNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalBinaryNode(sp::NodeType::LOr, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LogicalOrNode(" << *left << ", " << *right << ")";
        return ss;
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

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LogicalNotNode(" << *cond_expr << ")";
        return ss;
    }
};
} // namespace sp