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

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    [[nodiscard]] auto get_postfix() const -> std::string override;
    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
};

class VarNode : public ExprNode, public DesignEntitiesMixin {
  public:
    std::string name;

    explicit VarNode(std::string name) : ExprNode(NodeType::Variable), name(std::move(name)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    [[nodiscard]] auto get_postfix() const -> std::string override;
    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
};
} // namespace sp
