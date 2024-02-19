#include "common/ast/statement_ast.hpp"

namespace sp {

auto ReadNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {};
}

[[nodiscard]] auto ReadNode::get_node_name() const -> std::string {
    return "ReadNode";
}

[[nodiscard]] auto ReadNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << variable << ")";
    return ss;
}

[[nodiscard]] auto ReadNode::to_xml() const -> std::string {
    return "<" + get_node_name() + " stmt_number=\"" + std::to_string(get_statement_number()) + "\" variable=\"" +
           variable + "\" />";
}

auto ReadNode::populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    write_facade->add_statement(std::to_string(get_statement_number()), StatementType::Read);
}

auto ReadNode::populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade, std::shared_ptr<ModifyMap>)
    -> std::unordered_set<std::string> {
    auto stmt_number = std::to_string(get_statement_number());
    write_facade->add_statement_modifies_var(stmt_number, variable);

    return {variable};
}
} // namespace sp
