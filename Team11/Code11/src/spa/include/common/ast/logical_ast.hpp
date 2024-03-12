#pragma once

#include "common/ast/ast.hpp"

namespace sp {

/**
 * @brief LogicalNode is a ASTNode represents a logical operation. 
 */ 
class LogicalNode : public AstNode {
    using AstNode::AstNode;
};

/**
 * @brief LogicalBinaryNode is a ASTNode represents a binary logical operation.
 * @note In short, LogicalBinaryNode represents a generic Logical Expression with 2 operands.
 
 */
class LogicalBinaryNode : public LogicalNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit LogicalBinaryNode(sp::NodeType T, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalNode(T), left(std::move(left)), right(std::move(right)) {}
    virtual ~LogicalBinaryNode() = default;

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
};

/**
 * @brief LogicalAndNode represents a AST Node that is a logical AND operation.
 */
class LogicalAndNode : public LogicalBinaryNode {
  public:
    explicit LogicalAndNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalBinaryNode(sp::NodeType::LAnd, std::move(left), std::move(right)) {}

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LogicalAndNode";
    }
};

/**
 * @brief LogicalOrNode represents a AST Node that is a logical OR operation.
 */
class LogicalOrNode : public LogicalBinaryNode {
  public:
    explicit LogicalOrNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalBinaryNode(sp::NodeType::LOr, std::move(left), std::move(right)) {}

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "LogicalOrNode";
    }
};

/**
 * @brief LogicalNotNode represents a AST Node that is a logical NOT operation.
 */
class LogicalNotNode : public LogicalNode {
  public:
    std::shared_ptr<AstNode> cond_expr;

    explicit LogicalNotNode(std::shared_ptr<AstNode>& cond_expr)
        : LogicalNode(sp::NodeType::LNot), cond_expr(cond_expr) {}

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
};
} // namespace sp