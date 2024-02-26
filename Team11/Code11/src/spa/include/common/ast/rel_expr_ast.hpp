#pragma once

#include "common/ast/ast.hpp"

namespace sp {

class ComparatorNode : public sp::AstNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit ComparatorNode(NodeType T, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : AstNode(T), left(std::move(left)), right(std::move(right)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;

    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
};

class GreaterThanNode : public ComparatorNode {
  public:
    explicit GreaterThanNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Gt, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "GreaterThanNode";
    }
};

class GreaterThanEqualNode : public ComparatorNode {
  public:
    explicit GreaterThanEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Gte, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "GreaterThanEqualNode";
    }
};

class LessThanNode : public ComparatorNode {
  public:
    explicit LessThanNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Lt, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LessThanNode";
    }
};

class LessThanEqualNode : public ComparatorNode {
  public:
    explicit LessThanEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Lte, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LessThanEqualNode";
    }
};

class EqualNode : public ComparatorNode {
  public:
    explicit EqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Eq, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "EqualNode";
    }
};

class NotEqualNode : public ComparatorNode {
  public:
    explicit NotEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Neq, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "NotEqualNode";
    }
};
} // namespace sp
