#pragma once

#include "common/ast/ast.hpp"

namespace sp {

/**
 * @brief ComparatorNode is an AST Node represents a Comparison expression. (e.g. <, ==)
 */
class ComparatorNode : public sp::AstNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit ComparatorNode(NodeType T, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : AstNode(T), left(std::move(left)), right(std::move(right)) {
    }

    ~ComparatorNode() override = default;

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
};

/**
 * @brief GreaterThanNode represents a AST Node that is a greater than (>) operation.
 */
class GreaterThanNode : public ComparatorNode {
  public:
    explicit GreaterThanNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Gt, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "GreaterThanNode";
    }
};

/**
 * @brief GreaterThanEqualNode represents a AST Node that is a greater than or equal (>=) operation.
 */
class GreaterThanEqualNode : public ComparatorNode {
  public:
    explicit GreaterThanEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Gte, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "GreaterThanEqualNode";
    }
};

/**
 * @brief LessThanNode represents a AST Node that is a less than (<) operation.
 */
class LessThanNode : public ComparatorNode {
  public:
    explicit LessThanNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Lt, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LessThanNode";
    }
};

/**
 * @brief LessThanEqualNode represents a AST Node that is a less than or equal (<=) operation.
 */
class LessThanEqualNode : public ComparatorNode {
  public:
    explicit LessThanEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Lte, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LessThanEqualNode";
    }
};

/**
 * @brief EqualNode represents a AST Node that is a equal (==) operation.
 */
class EqualNode : public ComparatorNode {
  public:
    explicit EqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Eq, std::move(left), std::move(right)) {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "EqualNode";
    }
};

/**
 * @brief NotEqualNode represents a AST Node that is a not equal (!=) operation.
 */
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
