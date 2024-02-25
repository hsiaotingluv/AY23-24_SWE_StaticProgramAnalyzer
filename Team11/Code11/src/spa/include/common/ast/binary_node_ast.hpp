#pragma once

#include <utility>

#include "common/ast/ast.hpp"
#include "common/ast/mixin/expr_mixin.hpp"

namespace sp {

// Generic Binary Expression with 2 operands
class BinopNode : public ExprNode {
  public:
    std::shared_ptr<ExprNode> left{}, right{};
    std::string operator_token;

    explicit BinopNode(NodeType T, std::string token) : ExprNode(T), operator_token(std::move(token)) {
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

    [[nodiscard]] auto get_postfix() const -> std::string override {
        auto left_postfix = left->get_postfix();
        auto right_postfix = right->get_postfix();
        return left_postfix + " " + right_postfix + " " + operator_token + " ";
    }
};

class MulNode : public BinopNode {
  public:
    explicit MulNode() : BinopNode(NodeType::Mul, "*") {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "MulNode";
    }
};

class DivNode : public BinopNode {
  public:
    explicit DivNode() : BinopNode(NodeType::Div, "/") {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "DivNode";
    }
};

class ModNode : public BinopNode {
  public:
    explicit ModNode() : BinopNode(NodeType::Mod, "%") {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "ModNode";
    }
};

class AddNode : public BinopNode {
  public:
    explicit AddNode() : BinopNode(NodeType::Add, "+") {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "AddNode";
    }
};

class SubNode : public BinopNode {
  public:
    explicit SubNode() : BinopNode(NodeType::Sub, "-") {
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "SubNode";
    }
};
} // namespace sp
