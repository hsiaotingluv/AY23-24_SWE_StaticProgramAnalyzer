#include "common/ast/procedure_ast.hpp"

namespace sp {
auto ProcedureNode::populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    write_facade->add_procedure(proc_name);
}

auto ProcedureNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {stmt_list};
}

[[nodiscard]] auto ProcedureNode::get_node_name() const -> std::string {
    return "ProcedureNode";
}

[[nodiscard]] auto ProcedureNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << proc_name << ", " << *stmt_list << ")";
    return ss;
}

[[nodiscard]] auto ProcedureNode::to_xml() const -> std::string {
    auto xml = std::string("<" + get_node_name() + " name=\"" + proc_name + "\">");
    xml += stmt_list->to_xml();
    xml += "</" + get_node_name() + ">";
    return xml;
}

auto ProcedureNode::populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade,
                                          std::shared_ptr<ModifyMap> modify_map) -> std::unordered_set<std::string> {
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

auto ProcedureNode::get_vars_from_stmt_list(const std::shared_ptr<WriteFacade>& write_facade,
                                            const std::shared_ptr<UsesMap>& uses_map,
                                            const std::shared_ptr<StatementListNode>& node)
    -> std::unordered_set<std::string> {
    auto combined_set = std::unordered_set<std::string>();
    auto stmts = node->statements;
    for (const auto& stmt_node : stmts) {
        if (!MixinTypeChecker::is_uses_mixin_node(stmt_node)) {
            continue;
        }
        auto uses_mixin_node = std::dynamic_pointer_cast<UsesMixin>(stmt_node);
        auto vars_set = uses_mixin_node->populate_pkb_uses(write_facade, uses_map);
        for (const auto& var_name : vars_set) {
            combined_set.insert(var_name);
        }
    }
    return combined_set;
}

auto ProcedureNode::populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                                      std::shared_ptr<UsesMap> uses_map) const -> std::unordered_set<std::string> {
    // Uses(p, v) holds if there is a statement s in p
    auto var_names_stmt_list = get_vars_from_stmt_list(write_facade, uses_map, stmt_list);
    for (const auto& var_name : var_names_stmt_list) {
        write_facade->add_procedure_uses_var(proc_name, var_name);
    }
    uses_map->insert(std::make_pair(proc_name, var_names_stmt_list));
    return var_names_stmt_list;
}
} // namespace sp