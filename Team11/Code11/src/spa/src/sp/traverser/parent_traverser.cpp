#include "sp/traverser/parent_traverser.hpp"

namespace sp {

auto ParentTraverser::traverse_helper(const std::shared_ptr<AstNode>& node) -> void {
    if (NodeTypeChecker::is_if_node(node)) {
        get_parent_if(node);
    } else if (NodeTypeChecker::is_while_node(node)) {
        get_parent_while(node);
    }
}

auto ParentTraverser::get_stmt_nums_from_stmt_lst(const std::shared_ptr<AstNode>& node)
    -> std::unordered_set<std::string> {
    auto stmt_nums = std::unordered_set<std::string>{}; // Set of statement numbers

    auto stmt_list_node = std::dynamic_pointer_cast<StatementListNode>(node);
    auto statements = stmt_list_node->statements;

    for (const auto& stmt : statements) {
        auto stmt_node = std::dynamic_pointer_cast<StatementNode>(stmt);
        auto stmt_num = std::to_string(stmt_node->get_statement_number());

        stmt_nums.insert(stmt_num);
    }

    return stmt_nums;
}

auto ParentTraverser::get_parent_while(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    auto while_node = std::dynamic_pointer_cast<WhileNode>(node);
    auto parent_stmt_number = std::to_string(while_node->get_statement_number());

    // Traverse the statement list
    const auto stmt_list = while_node->stmt_list;
    auto children_stmt_num = get_stmt_nums_from_stmt_lst(stmt_list);
    for (const auto& child_stmt_num : children_stmt_num) {
        write_facade->add_parent(parent_stmt_number, child_stmt_num);
    }

    return children_stmt_num;
}

auto ParentTraverser::get_parent_if(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string> {
    auto if_node = std::dynamic_pointer_cast<IfNode>(node);
    auto parent_stmt_number = std::to_string(if_node->get_statement_number());

    auto combined_set = std::unordered_set<std::string>();

    // Traverse the then statement list
    auto then_stmt_list = std::dynamic_pointer_cast<StatementListNode>(if_node->then_stmt_list);
    auto children_then_stmt_num = get_stmt_nums_from_stmt_lst(then_stmt_list);
    for (const auto& child_stmt_num : children_then_stmt_num) {
        combined_set.insert(child_stmt_num);
    }

    // Traverse the else statement list
    auto else_stmt_list = std::dynamic_pointer_cast<StatementListNode>(if_node->else_stmt_list);
    auto children_else_stmt_num = get_stmt_nums_from_stmt_lst(else_stmt_list);
    for (const auto& child_stmt_num : children_else_stmt_num) {
        combined_set.insert(child_stmt_num);
    }

    for (const auto& child_stmt_num : combined_set) {
        write_facade->add_parent(parent_stmt_number, child_stmt_num);
    }
    return combined_set;
}

auto ParentTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>&)
    -> std::shared_ptr<AstNode> {
    traverse_helper(node);

    for (const auto& child : node->get_children()) {
        traverse(child, {});
    }

    return node;
}
} // namespace sp