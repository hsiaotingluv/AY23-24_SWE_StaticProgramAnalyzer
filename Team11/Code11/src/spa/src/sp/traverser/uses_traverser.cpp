#include "sp/traverser/uses_traverser.hpp"

// TODO: File is getting big. To split into individual uses_by_statements.cpp

namespace sp {

auto UsesTraverser::traverse_expression(const std::shared_ptr<AstNode>& expr_node) -> std::unordered_set<std::string> {
    // Return a set of used variables in an expression.
    auto combined_set = std::unordered_set<std::string>();

    if (NodeTypeChecker::is_var_node(expr_node)) {
        // If expression is a variable, add to combined set.
        auto var_node = std::dynamic_pointer_cast<VarNode>(expr_node);
        auto var_name = var_node->name;
        combined_set.insert(var_name);
        return combined_set;
    }

    // Else, check for children and recurse through the children.
    for (const auto& child : expr_node->get_children()) {
        auto child_set = traverse_expression(child);
        for (auto&& var_name : child_set) {
            combined_set.insert(var_name);
        }
    }
    return combined_set;
}

auto UsesTraverser::traverse_stmt_lst(const std::shared_ptr<StatementListNode>& stmt_list_node)
    -> std::unordered_set<std::string> {
    // Return a set of used variables in a statement list.
    auto combined_set = std::unordered_set<std::string>();

    // for each statement in the statement list
    for (const auto& stmt_node : stmt_list_node->statements) {
        auto stmt_vars_set = traverse_helper(stmt_node); // Insert to their stmt_numbers here
        for (auto&& var_name : stmt_vars_set) {
            combined_set.insert(var_name);
        }
    }
    return combined_set; // Return the set of used variables in the statement list.
}

auto UsesTraverser::traverse_helper(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    if (NodeTypeChecker::is_assignment_node(node)) {
        return get_use_assignment(node);
    } else if (NodeTypeChecker::is_print_node(node)) {
        return get_use_print(node);
    } else if (NodeTypeChecker::is_while_node(node)) {
        return get_use_while(node);
    } else if (NodeTypeChecker::is_if_node(node)) {
        return get_use_if(node);
    } else if (NodeTypeChecker::is_proc_node(node)) {
        return get_use_proc(node);
    } else if (NodeTypeChecker::is_call_node(node)) {
        return get_use_call(node);
    }
    return {};
}

auto UsesTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
    -> std::shared_ptr<AstNode> {
    // adapt to Uses
    for (const auto& node : node->get_children()) {
        const auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        proc_map.insert(std::make_pair(proc_node->proc_name, proc_node));
    }

    // Traversing using topo sort order allow each traversal to check what's the modifies in each call stmt
    for (const auto& proc_order : proc_topo_sort) {
        auto proc_node = proc_map.at(proc_order);
        traverse_helper(proc_node);
    }

    return node;
}

auto UsesTraverser::get_use_assignment(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Uses(a, v) holds if v appears on the right hand side of a.
    auto assignment_node = std::dynamic_pointer_cast<AssignmentNode>(node);
    auto stmt_number = std::to_string(assignment_node->get_statement_number());
    auto var_names = traverse_expression(assignment_node->expr);

    for (const auto& var_name : var_names) {
        write_facade->add_statement_uses_var(stmt_number, var_name);
    }
    return var_names;
}

auto UsesTraverser::get_use_print(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Uses(pn, v) holds if variable v appears in pn.
    auto print_node = std::dynamic_pointer_cast<PrintNode>(node);
    auto stmt_number = std::to_string(print_node->get_statement_number());
    auto var_name = print_node->variable;
    write_facade->add_statement_uses_var(stmt_number, var_name);
    return {var_name};
}

auto UsesTraverser::get_use_while(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Uses(s, v) holds if v appears in the condition of s
    auto while_node = std::dynamic_pointer_cast<WhileNode>(node);
    auto stmt_number = std::to_string(while_node->get_statement_number());

    auto combined_set = std::unordered_set<std::string>();

    // Traverse the conditional expression
    auto cond_expr = while_node->cond_expr;
    auto var_names_cond_expr = traverse_expression(cond_expr);
    for (const auto& var_name : var_names_cond_expr) {
        combined_set.insert(var_name);
    }

    // Traverse the statement list
    auto stmt_list = std::dynamic_pointer_cast<StatementListNode>(while_node->stmt_list);
    auto var_names_stmt_list = traverse_stmt_lst(stmt_list);
    for (const auto& var_name : var_names_stmt_list) {
        combined_set.insert(var_name);
    }

    // Return consolidated variables
    for (const auto& var_name : combined_set) {
        write_facade->add_statement_uses_var(stmt_number, var_name);
    }

    return combined_set;
}

auto UsesTraverser::get_use_if(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Uses(s, v) holds if v appears in the condition of s
    auto if_node = std::dynamic_pointer_cast<IfNode>(node);
    auto stmt_number = std::to_string(if_node->get_statement_number());

    auto combined_set = std::unordered_set<std::string>();

    // Traverse the conditional expression
    auto cond_expr = if_node->cond_expr;
    auto var_names_cond_expr = traverse_expression(cond_expr);
    for (const auto& var_name : var_names_cond_expr) {
        combined_set.insert(var_name);
    }

    // Traverse the then statement list
    auto then_stmt_list = std::dynamic_pointer_cast<StatementListNode>(if_node->then_stmt_list);
    auto var_names_then_stmt_list = traverse_stmt_lst(then_stmt_list);
    for (const auto& var_name : var_names_then_stmt_list) {
        combined_set.insert(var_name);
    }

    // Traverse the else statement list
    auto else_stmt_list = std::dynamic_pointer_cast<StatementListNode>(if_node->else_stmt_list);
    auto var_name_else_stmt_list = traverse_stmt_lst(else_stmt_list);
    for (const auto& var_name : var_name_else_stmt_list) {
        combined_set.insert(var_name);
    }

    // Return consolidated variables
    for (const auto& var_name : combined_set) {
        write_facade->add_statement_uses_var(stmt_number, var_name);
    }

    return combined_set;
}

auto UsesTraverser::get_use_proc(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Uses(p, v) holds if there is a statement s in p
    auto procedure_node = std::dynamic_pointer_cast<ProcedureNode>(node);
    auto proc_name = procedure_node->proc_name;

    // Traverse the statement list
    auto stmt_list = std::dynamic_pointer_cast<StatementListNode>(procedure_node->stmt_list);
    auto var_names_stmt_list = traverse_stmt_lst(stmt_list);
    for (const auto& var_name : var_names_stmt_list) {
        write_facade->add_procedure_uses_var(proc_name, var_name);
    }

    uses_map.insert({proc_name, var_names_stmt_list}); // For Calls statement

    return var_names_stmt_list;
}

auto UsesTraverser::get_use_call(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Uses(p, v) holds if there is a statement s in p
    auto call_node = std::dynamic_pointer_cast<CallNode>(node);
    auto stmt_number = std::to_string(call_node->get_statement_number());
    auto proc_name = call_node->proc_name;
    auto var_names_proc_name = uses_map.at(proc_name);

    for (const auto& var_name : var_names_proc_name) {
        write_facade->add_statement_uses_var(stmt_number, var_name);
    }

    return var_names_proc_name;
}
} // namespace sp