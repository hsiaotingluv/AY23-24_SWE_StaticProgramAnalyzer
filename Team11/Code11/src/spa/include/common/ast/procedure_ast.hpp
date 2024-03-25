#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/mixin/design_entities_mixin.hpp"
#include "common/ast/mixin/mixin_type_checker.hpp"
#include "common/ast/mixin/modifies_mixin.hpp"
#include "common/ast/mixin/uses_mixin.hpp"
#include "common/ast/statement_list_ast.hpp"

namespace sp {

/**
 * @brief ProcedureNode represents an AST Node that is a Procedure.
 */
class ProcedureNode : public AstNode, public DesignEntitiesMixin, public ModifiesMixin, public UsesMixin {
    static auto get_vars_from_stmt_list(const std::shared_ptr<pkb::WriteFacade>& write_facade,
                                        const std::shared_ptr<UsesMap>& uses_map,
                                        const std::shared_ptr<StatementListNode>& node)
        -> std::unordered_set<std::string>; // TODO: Move this function to a proper location, either public or private!

  public:
    std::string proc_name;
    std::shared_ptr<StatementListNode> stmt_list;

    explicit ProcedureNode(std::string proc_name, std::shared_ptr<StatementListNode>& stmt_list)
        : AstNode(sp::NodeType::Procedure), proc_name(std::move(proc_name)), stmt_list(stmt_list) {
    }

    // ASTNode Methods
    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;

    // DesignEntitiesMixin methods.
    auto populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void override;

    // ModifiesMixin methods.
    auto populate_pkb_modifies(const std::shared_ptr<pkb::WriteFacade>& write_facade,
                               const std::shared_ptr<ModifyMap>& modify_map)
        -> std::unordered_set<std::string> override;

    // UsesMixin methods.
    auto populate_pkb_uses(const std::shared_ptr<pkb::WriteFacade>& write_facade,
                           const std::shared_ptr<UsesMap>& uses_map) const -> std::unordered_set<std::string> override;
};
} // namespace sp
