#include "common/ast/node_type_checker.hpp"
#include "common/ast/statement_ast.hpp"

namespace sp {

auto PrintNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {var_node};
}

[[nodiscard]] auto PrintNode::get_node_name() const -> std::string {
    return "PrintNode";
}

[[nodiscard]] auto PrintNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << *var_node << ")";
    return ss;
}

[[nodiscard]] auto PrintNode::to_xml() const -> std::string {
    auto content = var_node->to_xml();
    auto start_xml = "<" + get_node_name() + " stmt_number=\"" + std::to_string(get_statement_number()) + "\">";
    auto end_xml = "</" + get_node_name() + ">";
    return start_xml + content + end_xml;
}

auto PrintNode::populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    write_facade->add_statement(std::to_string(get_statement_number()), StatementType::Print);
}

auto PrintNode::populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                                  const std::shared_ptr<UsesMap>&) const -> std::unordered_set<std::string> {
    // Uses(pn, v) holds if variable v appears in pn.
    auto stmt_number = std::to_string(get_statement_number());
    write_facade->add_statement_uses_var(stmt_number, var_node->name);
    return {var_node->name};
}

} // namespace sp
