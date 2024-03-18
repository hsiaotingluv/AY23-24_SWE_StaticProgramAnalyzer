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

auto CallNode::populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void {
    auto stmt_no = std::to_string(get_statement_number());
    write_facade->add_statement(stmt_no, StatementType::Call);
    write_facade->add_stmt_no_proc_called_mapping(stmt_no, proc_name);
}

auto CallNode::populate_pkb_modifies(const std::shared_ptr<pkb::WriteFacade>& write_facade,
                                     const std::shared_ptr<ModifyMap>& modify_map) -> std::unordered_set<std::string> {
    // Modifies(c,v)
    auto stmt_number = std::to_string(get_statement_number());
    auto modified_vars = modify_map->at(proc_name);
    for (const auto& var : modified_vars) {
        write_facade->add_statement_modify_var(stmt_number, var);
    }

    return modified_vars;
}

auto CallNode::populate_pkb_uses(const std::shared_ptr<pkb::WriteFacade>& write_facade,
                                 const std::shared_ptr<UsesMap>& uses_map) const -> std::unordered_set<std::string> {
    // Uses(p, v) holds if there is a statement s in p.
    auto stmt_number = std::to_string(get_statement_number());
    auto var_names_proc_name = uses_map->at(proc_name);
    for (const auto& var : var_names_proc_name) {
        write_facade->add_statement_use_var(stmt_number, var);
    }
    return var_names_proc_name;
}

auto CallNode::build_cfg(std::shared_ptr<ProcedureCfg> cfg) -> void {
    auto stmt_num = get_statement_number();
    cfg->add_stmt_to_node(stmt_num);
}

} // namespace sp
