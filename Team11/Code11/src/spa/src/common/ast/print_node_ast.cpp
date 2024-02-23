#include "common/ast/statement_ast.hpp"
#include "common/ast/node_type_checker.hpp"

namespace sp {

auto PrintNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {};
}

[[nodiscard]] auto PrintNode::get_node_name() const -> std::string {
    return "PrintNode";
}

[[nodiscard]] auto PrintNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << variable << ")";
    return ss;
}

[[nodiscard]] auto PrintNode::to_xml() const -> std::string {
    return "<" + get_node_name() + " stmt_number=\"" + std::to_string(get_statement_number()) + "\" variable=\"" +
           variable + "\" />";
}

auto PrintNode::populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    write_facade->add_statement(std::to_string(get_statement_number()), StatementType::Print);
}

auto PrintNode::populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade, std::shared_ptr<UsesMap>) const -> std::unordered_set<std::string> {
    // Uses(pn, v) holds if variable v appears in pn.
    auto stmt_number = std::to_string(get_statement_number());
    write_facade->add_statement_uses_var(stmt_number, variable);
    return {variable};
}

} // namespace sp
