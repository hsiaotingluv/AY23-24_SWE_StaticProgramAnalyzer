#pragma once

#include <utility>

#include "common/ast/ast.hpp"
#include "common/ast/mixin/design_entities_mixin.hpp"
#include "common/ast/mixin/expr_mixin.hpp"
#include "common/ast/node_type.hpp"

namespace sp {

/**
 * @brief ConstantNode is an AST Node represents a constant.
 */
class ConstantNode : public ExprNode, public DesignEntitiesMixin {
  public:
    std::string integer;

    explicit ConstantNode(std::string& integer) : ExprNode(NodeType::Constant), integer(std::move(integer)) {
    }

    // ExprNode methods.
    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    [[nodiscard]] auto get_postfix() const -> std::string override;

    // DesignEntitiesMixin methods.
    auto populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void override;
};

/**
 * @brief VarNode is an AST Node represents a variable.
 */
class VarNode : public ExprNode, public DesignEntitiesMixin {
  public:
    std::string name;

    explicit VarNode(std::string name) : ExprNode(NodeType::Variable), name(std::move(name)) {
    }

    // ExprNode methods.
    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    [[nodiscard]] auto get_postfix() const -> std::string override;

    // DesignEntitiesMixin methods.
    auto populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void override;
};
} // namespace sp
