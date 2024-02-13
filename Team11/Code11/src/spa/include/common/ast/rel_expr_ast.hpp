#pragma once

#include "common/ast/ast.hpp"

namespace sp {

class ComparatorNode : public sp::AstNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit ComparatorNode(NodeType T, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : left(std::move(left)), right(std::move(right)), AstNode(T) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {left, right};
    }
};

class GreaterThanNode : public ComparatorNode {
  public:
    explicit GreaterThanNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Gt, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "GreaterThanNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class GreaterThanEqualNode : public ComparatorNode {
  public:
    explicit GreaterThanEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Gte, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "GreaterThanEqualNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class LessThanNode : public ComparatorNode {
  public:
    explicit LessThanNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Lt, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LessThanNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class LessThanEqualNode : public ComparatorNode {
  public:
    explicit LessThanEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Lte, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LessThanEqualNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class EqualNode : public ComparatorNode {
  public:
    explicit EqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Eq, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "EqualNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class NotEqualNode : public ComparatorNode {
  public:
    explicit NotEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Neq, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "NotEqualNode(" << *left << ", " << *right << ")";
        return ss;
    }
};
} // namespace sp
