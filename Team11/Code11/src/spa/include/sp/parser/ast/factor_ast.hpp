#pragma once

#include "sp/parser/ast/ast.hpp"
#include "sp/parser/ast/node_type.hpp"

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

class NameNode : public AstNode {
  public:
    std::string name;

    explicit NameNode(std::string name) : name(std::move(name)), AstNode(NodeType::Variable) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "NameNode(" << name << ")";
        return ss;
    }
};
} // namespace sp
