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

auto IfNode::get_stmt_nums(const std::shared_ptr<StatementListNode>& node) const -> std::unordered_set<std::string> {
    // Consider only level-1 children. Processing further nested children are handled by PKB.
    auto statement_nums = std::unordered_set<std::string>{};
    auto statements = node->statements;
    for (const auto& statement: statements) {
        auto statement_node = std::dynamic_pointer_cast<StatementNode>(statement);
        auto statement_num = std::to_string(statement_node->get_statement_number());
        statement_nums.insert(statement_num);
    }
    return statement_nums;
}

auto IfNode::populate_pkb_parent(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    auto parent_statement_num = std::to_string(get_statement_number());

    auto then_statement_nums = get_stmt_nums(then_stmt_list);
    for (const auto& child_statement_num : then_statement_nums) {
        write_facade->add_parent(parent_statement_num, child_statement_num);
    }

    auto else_statement_nums = get_stmt_nums(else_stmt_list);
    for (const auto& child_statement_num : else_statement_nums) {
        write_facade->add_parent(parent_statement_num, child_statement_num);
    }
}

} // namespace sp
