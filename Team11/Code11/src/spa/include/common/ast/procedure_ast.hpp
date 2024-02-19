#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/mixin/design_entities_mixin.hpp"
#include "common/ast/mixin/modifies_mixin.hpp"
#include "common/ast/statement_list_ast.hpp"

namespace sp {

class ProcedureNode : public AstNode, public DesignEntitiesMixin, public ModifiesMixin {

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
};
} // namespace sp
