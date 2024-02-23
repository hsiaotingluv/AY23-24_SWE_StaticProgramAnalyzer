#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/mixin/design_entities_mixin.hpp"
#include "common/ast/mixin/modifies_mixin.hpp"
#include "common/ast/mixin/uses_mixin.hpp"
#include "common/ast/mixin/mixin_type_checker.hpp"
#include "common/ast/statement_list_ast.hpp"

namespace sp {

class ProcedureNode : public AstNode, public DesignEntitiesMixin, public ModifiesMixin, public UsesMixin {

  public:
    std::string proc_name;
    std::shared_ptr<StatementListNode> stmt_list;

    explicit ProcedureNode(std::string proc_name, std::shared_ptr<StatementListNode>& stmt_list)
        : AstNode(sp::NodeType::Procedure), proc_name(std::move(proc_name)), stmt_list(stmt_list) {
    }

    auto populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void override {
        write_facade->add_procedure(proc_name);
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {stmt_list};
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "ProcedureNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << proc_name << ", " << *stmt_list << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        auto xml = std::string("<" + get_node_name() + " name=\"" + proc_name + "\">");
        xml += stmt_list->to_xml();
        xml += "</" + get_node_name() + ">";
        return xml;
    }

    auto populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade, std::shared_ptr<ModifyMap> modify_map)
        -> std::unordered_set<std::string> override {
        // Modifies(p, v)
        auto combined_set = std::unordered_set<std::string>();

        for (const auto& stmt_node : stmt_list->statements) {
            auto maybe_modify_node = std::dynamic_pointer_cast<ModifiesMixin>(stmt_node);
            if (maybe_modify_node == nullptr) {
                continue;
            }
            auto stmt_vars_set = maybe_modify_node->populate_pkb_modifies(write_facade, modify_map);
            for (auto&& var : stmt_vars_set) {
                combined_set.insert(var);
            }
        }

        for (const auto& var : combined_set) {
            write_facade->add_procedure_modifies_var(proc_name, var);
        }

        modify_map->insert(std::make_pair(proc_name, combined_set));
        return combined_set;
    }

    auto get_vars_from_stmt_list(const std::shared_ptr<WriteFacade>& write_facade, std::shared_ptr<UsesMap> uses_map, const std::shared_ptr<StatementListNode>& node) const -> std::unordered_set<std::string> {
        auto combined_set = std::unordered_set<std::string>();
        auto stmts = node->statements;
        std::for_each(stmts.begin(), stmts.end(), [&](const auto& stmt_node) {
            if (!MixinTypeChecker::is_uses_mixin_node(stmt_node)) {
                return;
            }
            auto uses_mixin_node = std::dynamic_pointer_cast<UsesMixin>(stmt_node);
            auto vars_set = uses_mixin_node->populate_pkb_uses(write_facade, uses_map);
            std::for_each(vars_set.begin(), vars_set.end(), [&](const auto& var_name) {
                combined_set.insert(var_name);
            });
        });
        return combined_set;
    }

    auto populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade, std::shared_ptr<UsesMap> uses_map) const -> std::unordered_set<std::string> override {
        // Uses(p, v) holds if there is a statement s in p
        auto var_names_stmt_list = get_vars_from_stmt_list(write_facade, uses_map, stmt_list);
        std::for_each(var_names_stmt_list.begin(), var_names_stmt_list.end(), [&](const auto& var_name) {
            write_facade->add_procedure_uses_var(proc_name, var_name);
        });
        uses_map->insert(std::make_pair(proc_name, var_names_stmt_list)); //Memoisation
        return var_names_stmt_list;
    }};
} // namespace sp
