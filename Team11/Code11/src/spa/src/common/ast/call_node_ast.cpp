#include "common/ast/statement_ast.hpp"

namespace sp {

auto CallNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {};
}

[[nodiscard]] auto CallNode::get_node_name() const -> std::string {
    return "CallNode";
}

[[nodiscard]] auto CallNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << proc_name << ")";
    return ss;
}

[[nodiscard]] auto CallNode::to_xml() const -> std::string {
    const auto stmt_number = std::to_string(get_statement_number());
    return "<" + get_node_name() + " stmt_number=\"" + stmt_number + "\" proc_name=\"" + proc_name + "\" />";
}

auto CallNode::populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    write_facade->add_statement(std::to_string(get_statement_number()), StatementType::Call);
}

auto CallNode::populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade,
                                     std::shared_ptr<ModifyMap> modify_map) -> std::unordered_set<std::string> {
    // Modifies(c,v)
    auto stmt_number = std::to_string(get_statement_number());
    auto modified_vars = modify_map->at(proc_name);
    for (const auto& var : modified_vars) {
        write_facade->add_statement_modifies_var(stmt_number, var);
    }

    return modified_vars;
}

auto CallNode::populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                                 const std::shared_ptr<UsesMap>& uses_map) const -> std::unordered_set<std::string> {
    // Uses(p, v) holds if there is a statement s in p.
    auto stmt_number = std::to_string(get_statement_number());
    auto var_names_proc_name = uses_map->at(proc_name);
    for (const auto& var : var_names_proc_name) {
        write_facade->add_statement_uses_var(stmt_number, var);
    }
    return var_names_proc_name;
}

} // namespace sp
