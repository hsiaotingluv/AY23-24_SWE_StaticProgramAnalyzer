#include "sp/traverser/modifies_traverser.hpp"

namespace sp {

/*
 * @brief Traverses proc node to populate the pkb and returns set of vars that are being modified
 *
 * @Note: there are some side effects such as updating the modifies_map and write_facade
 */
auto ModifiesTraverser::traverse_helper(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    if (NodeTypeChecker::is_assignment_node(node)) {
        return get_modify_assignment(node);
    } else if (NodeTypeChecker::is_read_node(node)) {
        return get_modify_read(node);
    } else if (NodeTypeChecker::is_call_node(node)) {
        return get_modify_call(node);
    } else if (NodeTypeChecker::is_while_node(node)) {
        return get_modify_while(node);
    } else if (NodeTypeChecker::is_if_node(node)) {
        return get_modify_if(node);
    } else if (NodeTypeChecker::is_proc_node(node)) {
        return get_modify_proc(node);
    }

    return {};
}

auto ModifiesTraverser::get_modify_if(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Modifies(s, v) for s = If
    auto if_node = std::dynamic_pointer_cast<IfNode>(node);
    auto stmt_number = std::to_string(if_node->get_statement_number());

    auto then_stmt_list_node = std::dynamic_pointer_cast<StatementListNode>(if_node->then_stmt_list);
    auto else_stmt_list_node = std::dynamic_pointer_cast<StatementListNode>(if_node->else_stmt_list);
    auto combined_set = std::unordered_set<std::string>();

    // trying to avoid combining then and else since it incurs copy ctors
    for (const auto& stmt_node : then_stmt_list_node->statements) {
        auto stmt_vars_set = traverse_helper(stmt_node);
        for (auto&& var : stmt_vars_set) {
            combined_set.insert(var);
        }
    }

    for (const auto& stmt_node : else_stmt_list_node->statements) {
        auto stmt_vars_set = traverse_helper(stmt_node);
        for (auto&& var : stmt_vars_set) {
            combined_set.insert(var);
        }
    }

    for (const auto& var : combined_set) {
        write_facade->add_statement_modifies_var(stmt_number, var);
    }

    return combined_set;
}

auto ModifiesTraverser::get_modify_while(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Modifies(s, v) for s = While
    auto while_node = std::dynamic_pointer_cast<WhileNode>(node);
    auto stmt_number = std::to_string(while_node->get_statement_number());
    auto stmt_list_node = std::dynamic_pointer_cast<StatementListNode>(while_node->stmt_list);

    auto combined_set = std::unordered_set<std::string>();

    for (const auto& stmt_node : stmt_list_node->statements) {
        auto stmt_vars_set = traverse_helper(stmt_node);
        for (auto&& var : stmt_vars_set) {
            combined_set.insert(var);
        }
    }

    for (const auto& var : combined_set) {
        write_facade->add_statement_modifies_var(stmt_number, var);
    }

    return combined_set;
}

auto ModifiesTraverser::get_modify_call(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Modifies(c,v)
    auto call_node = std::dynamic_pointer_cast<CallNode>(node);
    auto stmt_number = std::to_string(call_node->get_statement_number());
    auto modified_vars = modifies_map.at(call_node->proc_name);
    for (const auto& var : modified_vars) {
        write_facade->add_statement_modifies_var(stmt_number, var);
    }

    return modified_vars;
}

auto ModifiesTraverser::get_modify_read(const std::shared_ptr<AstNode>& node)
    -> std::unordered_set<std::string> { // Modifies(re, v)
    auto read_node = std::dynamic_pointer_cast<ReadNode>(node);
    auto stmt_number = std::to_string(read_node->get_statement_number());
    write_facade->add_statement_modifies_var(stmt_number, read_node->variable);

    return {read_node->variable};
}

auto ModifiesTraverser::get_modify_assignment(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Modifies(a, v)
    auto assignment_node = std::dynamic_pointer_cast<AssignmentNode>(node);
    auto stmt_number = std::to_string(assignment_node->get_statement_number());
    auto var_node = std::dynamic_pointer_cast<VarNode>(assignment_node->variable);
    write_facade->add_statement_modifies_var(stmt_number, var_node->name);

    return {var_node->name};
}

auto ModifiesTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
    -> std::shared_ptr<AstNode> {

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

auto ModifiesTraverser::get_modify_proc(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    // Modifies(p, v)
    auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
    auto stmt_list_node = std::dynamic_pointer_cast<StatementListNode>(proc_node->stmt_list);
    auto combined_set = std::unordered_set<std::string>();

    for (const auto& stmt_node : stmt_list_node->statements) {
        auto stmt_vars_set = traverse_helper(stmt_node);
        for (auto&& var : stmt_vars_set) {
            combined_set.insert(var);
        }
    }

    for (const auto& var : combined_set) {
        write_facade->add_procedure_modifies_var(proc_node->proc_name, var);
    }

    modifies_map.insert(std::make_pair(proc_node->proc_name, combined_set));
    return combined_set;
}
} // namespace sp
