#include "common/ast/node_type_checker.hpp"
#include "common/ast/statement_ast.hpp"

namespace sp {
auto AssignmentNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {variable, expr};
}

[[nodiscard]] auto AssignmentNode::get_node_name() const -> std::string {
    return "AssignmentNode";
}

[[nodiscard]] auto AssignmentNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << *variable << ", " << *expr << ")";
    return ss;
}

[[nodiscard]] auto AssignmentNode::to_xml() const -> std::string {
    auto var_xml = variable->to_xml();
    auto expr_xml = expr->to_xml();
    const auto stmt_number = std::to_string(get_statement_number());

    auto opening_xml = "<" + get_node_name() + " stmt_number=\"" + stmt_number + "\">";
    auto ending_xml = "</" + get_node_name() + ">";
    return opening_xml + var_xml + expr_xml + ending_xml;
}

auto AssignmentNode::populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    auto lhs = variable->name;
    auto rhs = expr->get_postfix();
    write_facade->add_assignment(std::to_string(get_statement_number()), lhs, rhs);
    write_facade->add_statement(std::to_string(get_statement_number()), StatementType::Assign);
}

auto AssignmentNode::populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade, std::shared_ptr<ModifyMap>)
    -> std::unordered_set<std::string> {
    // Modifies(a, v)
    auto stmt_number = std::to_string(get_statement_number());
    auto var_node = std::dynamic_pointer_cast<VarNode>(variable);
    write_facade->add_statement_modifies_var(stmt_number, var_node->name);

    return {var_node->name};
}

auto AssignmentNode::get_vars_from_expr(const std::shared_ptr<AstNode>& node) const -> std::unordered_set<std::string> {
    // Get all variable names from an expression.
    if (NodeTypeChecker::is_var_node(node)) {
        // If expression is a variable, add to combined set.
        auto var_node = std::dynamic_pointer_cast<VarNode>(node);
        return {var_node->name};
    }

    auto combined_set = std::unordered_set<std::string>();
    auto children = node->get_children();
    std::for_each(children.begin(), children.end(), [&](const auto& child) {
        auto child_var_names = get_vars_from_expr(child); // Extract variable names from each child.
        std::for_each(child_var_names.begin(), child_var_names.end(), [&](const auto& var_name) {
            combined_set.insert(var_name); // Add each variable from each child to the combined set.
        });
    });
    return combined_set;
}

auto AssignmentNode::populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade, std::shared_ptr<UsesMap>) const
    -> std::unordered_set<std::string> {
    // Uses(a, v) holds if v appears on the right hand side of a.
    auto stmt_number = std::to_string(get_statement_number());
    auto var_names = get_vars_from_expr(expr);
    std::for_each(var_names.begin(), var_names.end(), [&](const auto& var_name) {
        write_facade->add_statement_uses_var(stmt_number, var_name);
    });
    return var_names;
}

} // namespace sp
