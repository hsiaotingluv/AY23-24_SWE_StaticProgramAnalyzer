#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/node_type.hpp"

namespace sp {

class ConstantNode : public AstNode {
  public:
    int integer;

    explicit ConstantNode(int integer) : integer(integer), AstNode(NodeType::Constant) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "ConstantNode(" << integer << ")";
        return ss;
    }
};

class VarNode : public AstNode {
  public:
    std::string name;

    explicit VarNode(std::string name) : name(std::move(name)), AstNode(NodeType::Variable) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "VarNode(" << name << ")";
        return ss;
    }
};
} // namespace sp
