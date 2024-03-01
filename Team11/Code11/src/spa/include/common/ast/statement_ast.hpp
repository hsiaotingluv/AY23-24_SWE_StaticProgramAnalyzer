#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/mixin/cfg_mixin.hpp"
#include "common/ast/mixin/design_entities_mixin.hpp"
#include "common/ast/mixin/expr_mixin.hpp"
#include "common/ast/mixin/modifies_mixin.hpp"
#include "common/ast/mixin/parent_mixin.hpp"
#include "common/ast/mixin/uses_mixin.hpp"
#include "factor_ast.hpp"
#include "statement_list_ast.hpp"

#include <optional>

/* These nodes follow the specs on
 * https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/abstract-syntax-tree.html
 * It means that variable is not stored as a new name node, but as the content of read node itself
 */
namespace sp {

class StatementNode : public AstNode, public DesignEntitiesMixin, public CfgMixin {
    std::optional<uint32_t> statement_number;

  public:
    explicit StatementNode(sp::NodeType T) : AstNode(T) {
    }

    auto set_statement_number(uint32_t statement_number) -> void;
    [[nodiscard]] auto get_statement_number() const -> int;
    [[nodiscard]] auto has_statement_number() const -> bool;
};

class ReadNode : public StatementNode, public ModifiesMixin {
  public:
    std::shared_ptr<VarNode> var_node;

    explicit ReadNode(std::shared_ptr<VarNode> var_node)
        : StatementNode(sp::NodeType::Read), var_node(std::move(var_node)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
    auto populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade, const std::shared_ptr<ModifyMap>&)
        -> std::unordered_set<std::string> override;
    auto build_cfg(std::shared_ptr<Cfg> cfg) -> void override;
};

class PrintNode : public StatementNode, public UsesMixin {
  public:
    std::shared_ptr<VarNode> var_node;

    explicit PrintNode(std::shared_ptr<VarNode> var_node)
        : StatementNode(sp::NodeType::Print), var_node(std::move(var_node)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
    auto populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade, const std::shared_ptr<UsesMap>&) const
        -> std::unordered_set<std::string> override;
    auto build_cfg(std::shared_ptr<Cfg> cfg) -> void override;
};

class CallNode : public StatementNode, public ModifiesMixin, public UsesMixin {
  public:
    std::string proc_name;

    explicit CallNode(std::string variable) : StatementNode(sp::NodeType::Call), proc_name(std::move(variable)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
    auto populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade,
                               const std::shared_ptr<ModifyMap>& modify_map)
        -> std::unordered_set<std::string> override;
    auto populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                           const std::shared_ptr<UsesMap>& uses_map) const -> std::unordered_set<std::string> override;
    auto build_cfg(std::shared_ptr<Cfg> cfg) -> void override;
};

class IfNode : public StatementNode, public ModifiesMixin, public ParentMixin, public UsesMixin {
    [[nodiscard]] auto get_vars_from_expr(const std::shared_ptr<AstNode>& node) const
        -> std::unordered_set<std::string>;
    auto get_vars_from_stmt_list(const std::shared_ptr<WriteFacade>& write_facade,
                                 const std::shared_ptr<UsesMap>& uses_map,
                                 const std::shared_ptr<StatementListNode>& node) const
        -> std::unordered_set<std::string>;
    [[nodiscard]] static auto get_stmt_nums(const std::shared_ptr<StatementListNode>& node)
        -> std::unordered_set<std::string>;

  public:
    std::shared_ptr<AstNode> cond_expr;
    std::shared_ptr<StatementListNode> then_stmt_list;
    std::shared_ptr<StatementListNode> else_stmt_list;

    IfNode(std::shared_ptr<AstNode>& cond_expr, std::shared_ptr<StatementListNode>& then_stmt_list,
           std::shared_ptr<StatementListNode>& else_stmt_list)
        : StatementNode(sp::NodeType::If), cond_expr(cond_expr), then_stmt_list(then_stmt_list),
          else_stmt_list(else_stmt_list) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    auto populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade,
                               const std::shared_ptr<ModifyMap>& modify_map)
        -> std::unordered_set<std::string> override;
    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
    auto populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                           const std::shared_ptr<UsesMap>& uses_map) const -> std::unordered_set<std::string> override;
    auto populate_pkb_parent(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
    auto build_cfg(std::shared_ptr<Cfg> cfg) -> void override;
};

class WhileNode : public StatementNode, public ModifiesMixin, public ParentMixin, public UsesMixin {
    [[nodiscard]] auto get_vars_from_expr(const std::shared_ptr<AstNode>& node) const
        -> std::unordered_set<std::string>;
    [[nodiscard]] static auto get_vars_from_stmt_list(const std::shared_ptr<WriteFacade>& write_facade,
                                                      const std::shared_ptr<UsesMap>& uses_map,
                                                      const std::shared_ptr<StatementListNode>& node)
        -> std::unordered_set<std::string>;
    [[nodiscard]] static auto get_stmt_nums(const std::shared_ptr<StatementListNode>& node)
        -> std::unordered_set<std::string>;

  public:
    std::shared_ptr<AstNode> cond_expr;
    std::shared_ptr<StatementListNode> stmt_list;

    WhileNode(std::shared_ptr<AstNode>& cond_expr, std::shared_ptr<StatementListNode>& stmt_list)
        : StatementNode(sp::NodeType::While), cond_expr(cond_expr), stmt_list(stmt_list) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
    auto populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade,
                               const std::shared_ptr<ModifyMap>& modify_map)
        -> std::unordered_set<std::string> override;
    auto populate_pkb_parent(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
    auto populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                           const std::shared_ptr<UsesMap>& uses_map) const -> std::unordered_set<std::string> override;
    auto build_cfg(std::shared_ptr<Cfg> cfg) -> void override;
};

class AssignmentNode : public StatementNode, public ModifiesMixin, public UsesMixin {
  private:
    [[nodiscard]] auto get_vars_from_expr(const std::shared_ptr<AstNode>& node) const
        -> std::unordered_set<std::string>;

  public:
    std::shared_ptr<VarNode> variable;
    std::shared_ptr<ExprNode> expr;

    AssignmentNode(std::shared_ptr<VarNode> variable, std::shared_ptr<ExprNode> expr)
        : StatementNode(sp::NodeType::Assign), variable(std::move(variable)), expr(std::move(expr)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override;
    auto populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade, const std::shared_ptr<ModifyMap>&)
        -> std::unordered_set<std::string> override;
    auto populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                           const std::shared_ptr<UsesMap>& uses_map) const -> std::unordered_set<std::string> override;
    auto build_cfg(std::shared_ptr<Cfg> cfg) -> void override;
};
} // namespace sp
