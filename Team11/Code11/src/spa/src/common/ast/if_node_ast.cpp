#include "common/ast/mixin/mixin_type_checker.hpp"
#include "common/ast/node_type_checker.hpp"
#include "common/ast/statement_ast.hpp"

namespace sp {
auto IfNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {cond_expr, then_stmt_list, else_stmt_list};
}

[[nodiscard]] auto IfNode::get_node_name() const -> std::string {
    return "IfNode";
}

[[nodiscard]] auto IfNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << *cond_expr << ", " << *then_stmt_list << ", " << *else_stmt_list << ")";
    return ss;
}

[[nodiscard]] auto IfNode::to_xml() const -> std::string {
    auto cond_expr_xml = cond_expr->to_xml();
    auto then_xml = then_stmt_list->to_xml();
    auto else_xml = else_stmt_list->to_xml();
    const auto stmt_number = std::to_string(get_statement_number());
    auto opening_xml = "<" + get_node_name() + " stmt_number=\"" + stmt_number + "\">";
    auto ending_xml = "</" + get_node_name() + ">";
    return opening_xml + cond_expr_xml + then_xml + else_xml + ending_xml;
}

auto IfNode::populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade,
                                   std::shared_ptr<ModifyMap> modify_map) -> std::unordered_set<std::string> {
    // Modifies(s, v) for s = If
    auto stmt_number = std::to_string(get_statement_number());

    auto then_stmt_list_node = std::dynamic_pointer_cast<StatementListNode>(then_stmt_list);
    auto else_stmt_list_node = std::dynamic_pointer_cast<StatementListNode>(else_stmt_list);
    auto combined_set = std::unordered_set<std::string>();

    // trying to avoid combining then and else since it incurs copy ctors
    for (const auto& stmt_node : then_stmt_list_node->statements) {
        auto maybe_modify_node = std::dynamic_pointer_cast<ModifiesMixin>(stmt_node);
        if (maybe_modify_node == nullptr) {
            continue;
        }
        auto stmt_vars_set = maybe_modify_node->populate_pkb_modifies(write_facade, modify_map);
        for (auto&& var : stmt_vars_set) {
            combined_set.insert(var);
        }
    }

    for (const auto& stmt_node : else_stmt_list_node->statements) {
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
        write_facade->add_statement_modifies_var(stmt_number, var);
    }

    return combined_set;
}

auto IfNode::populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    write_facade->add_statement(std::to_string(get_statement_number()), StatementType::If);
}

auto IfNode::get_vars_from_expr(const std::shared_ptr<AstNode>& node) const -> std::unordered_set<std::string> {
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

auto IfNode::get_vars_from_stmt_list(const std::shared_ptr<WriteFacade>& write_facade,
                                     std::shared_ptr<UsesMap> uses_map,
                                     const std::shared_ptr<StatementListNode>& node) const
    -> std::unordered_set<std::string> {
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

auto IfNode::populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                               std::shared_ptr<UsesMap> uses_map) const -> std::unordered_set<std::string> {
    // Uses(s, v) for s = If
    auto stmt_number = std::to_string(get_statement_number());
    auto combined_set = std::unordered_set<std::string>();

    // Traverse the conditional expression
    auto var_names_cond_expr = get_vars_from_expr(cond_expr);
    std::for_each(var_names_cond_expr.begin(), var_names_cond_expr.end(), [&](const auto& var_name) {
        combined_set.insert(var_name);
    });

    // Traverse then statement list
    auto var_names_then_stmt_list = get_vars_from_stmt_list(write_facade, uses_map, then_stmt_list);
    std::for_each(var_names_then_stmt_list.begin(), var_names_then_stmt_list.end(), [&](const auto& var_name) {
        combined_set.insert(var_name);
    });

    // Traverse else statement list
    auto var_names_else_stmt_list = get_vars_from_stmt_list(write_facade, uses_map, else_stmt_list);
    std::for_each(var_names_else_stmt_list.begin(), var_names_else_stmt_list.end(), [&](const auto& var_name) {
        combined_set.insert(var_name);
    });

    // Add all variables to the PKB.
    for (const auto& var_name : combined_set) {
        write_facade->add_statement_uses_var(stmt_number, var_name);
    }

    return combined_set;
}

} // namespace sp
