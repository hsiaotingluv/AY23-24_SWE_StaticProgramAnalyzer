#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/node_type.hpp"

namespace sp {

class ConstantNode : public AstNode {
  public:
    int integer;

    explicit ConstantNode(int integer) : AstNode(NodeType::Constant), integer(integer) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "ConstantNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << integer << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        return "<" + get_node_name() + " integer=\"" + std::to_string(integer) + "\" />";
    }
};

class VarNode : public AstNode {
  public:
    std::string name;

    explicit VarNode(std::string name) : AstNode(NodeType::Variable), name(std::move(name)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "VarNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << name << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        return "<" + get_node_name() + " name=\"" + name + "\" />";
    }
};
} // namespace sp
