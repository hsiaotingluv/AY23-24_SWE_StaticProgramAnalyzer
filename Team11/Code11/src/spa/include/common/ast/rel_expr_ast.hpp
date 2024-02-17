#pragma once

#include "common/ast/ast.hpp"

namespace sp {

class ComparatorNode : public sp::AstNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit ComparatorNode(NodeType T, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : AstNode(T), left(std::move(left)), right(std::move(right)) {
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
