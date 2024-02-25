#pragma once

#include <utility>

#include "common/ast/ast.hpp"
#include "common/ast/mixin/design_entities_mixin.hpp"
#include "common/ast/mixin/expr_mixin.hpp"
#include "common/ast/node_type.hpp"

namespace sp {

class ConstantNode : public ExprNode, public DesignEntitiesMixin {
  public:
    std::string integer;

    explicit ConstantNode(std::string& integer) : ExprNode(NodeType::Constant), integer(std::move(integer)) {
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
        return "<" + get_node_name() + " integer=\"" + integer + "\" />";
    }

    [[nodiscard]] auto get_postfix() const -> std::string override {
        return integer + " ";
    }

    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override {
        write_facade->add_constant(integer);
    }
};

class VarNode : public ExprNode, public DesignEntitiesMixin {
  public:
    std::string name;

    explicit VarNode(std::string name) : ExprNode(NodeType::Variable), name(std::move(name)) {
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

    [[nodiscard]] auto get_postfix() const -> std::string override {
        return name + " ";
    }

    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override {
        write_facade->add_variable(name);
    }
};
} // namespace sp
