#pragma once

#include <utility>

#include "common/ast/ast.hpp"
#include "common/ast/mixin/expr_mixin.hpp"

namespace sp {

/**
 * @brief BinopNode is a ASTNode represents a binary operation.
 * @note In short, BinopNode represents a generic Binary Expression with 2 operands.
 */
class BinopNode : public ExprNode {
  public:
    std::shared_ptr<ExprNode> left{}, right{};
    std::string operator_token;

    explicit BinopNode(NodeType T, std::string token) : ExprNode(T), operator_token(std::move(token)) {}
    virtual ~BinopNode() = default;

    // AstNode methods.
    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;

    // ExprMixin methods.
    [[nodiscard]] auto get_postfix() const -> std::string override;
};

/**
 * @brief MulNode represents a AST Node that is a multiplication operation.
 */
class MulNode : public BinopNode {
  public:
    explicit MulNode() : BinopNode(NodeType::Mul, "*") {}

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "MulNode";
    }
};

/**
 * @brief DivNode represents a AST Node that is a division operation.
 */
class DivNode : public BinopNode {
  public:
    explicit DivNode() : BinopNode(NodeType::Div, "/") {}

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "DivNode";
    }
};

/**
 * @brief ModNode represents a AST Node that is a modulo operation.
 *
 */
class ModNode : public BinopNode {
  public:
    explicit ModNode() : BinopNode(NodeType::Mod, "%") {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "ModNode";
    }
};

/**
 * @brief AddNode represents a AST Node that is an addition operation.
 */
class AddNode : public BinopNode {
  public:
    explicit AddNode() : BinopNode(NodeType::Add, "+") {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "AddNode";
    }
};

/**
 * @brief SubNode represents a AST Node that is a subtraction operation.
 */
class SubNode : public BinopNode {
  public:
    explicit SubNode() : BinopNode(NodeType::Sub, "-") {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "SubNode";
    }
};
} // namespace sp
