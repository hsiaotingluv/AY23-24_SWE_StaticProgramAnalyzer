#include "common/ast/statement_ast.hpp"

namespace sp {

auto ReadNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {var_node};
}

[[nodiscard]] auto ReadNode::get_node_name() const -> std::string {
    return "ReadNode";
}

[[nodiscard]] auto ReadNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << *var_node << ")";
    return ss;
}

[[nodiscard]] auto ReadNode::to_xml() const -> std::string {
    auto content = var_node->to_xml();
    auto start_xml = "<" + get_node_name() + " stmt_number=\"" + std::to_string(get_statement_number()) + "\">";
    auto end_xml = "</" + get_node_name() + ">";
    return start_xml + content + end_xml;
}

auto ReadNode::populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void {
    write_facade->add_statement(std::to_string(get_statement_number()), StatementType::Read);
}

auto ReadNode::populate_pkb_modifies(const std::shared_ptr<pkb::WriteFacade>& write_facade,
                                     const std::shared_ptr<ModifyMap>&) -> std::unordered_set<std::string> {
    auto stmt_number = std::to_string(get_statement_number());
    write_facade->add_statement_modifies_var(stmt_number, var_node->name);

    return {var_node->name};
}

auto ReadNode::build_cfg(std::shared_ptr<Cfg> cfg) -> void {
    auto stmt_num = get_statement_number();
    cfg->add_stmt_to_node(stmt_num);
}
} // namespace sp
