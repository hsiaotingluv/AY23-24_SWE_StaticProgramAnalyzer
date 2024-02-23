#include "common/ast/statement_ast.hpp"
#include "common/ast/node_type_checker.hpp"

namespace sp {
auto WhileNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {cond_expr, stmt_list};
}

[[nodiscard]] auto WhileNode::get_node_name() const -> std::string {
    return "WhileNode";
}

[[nodiscard]] auto WhileNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << *cond_expr << ", " << *stmt_list << ")";
    return ss;
}

[[nodiscard]] auto WhileNode::to_xml() const -> std::string {
    auto cond_expr_xml = cond_expr->to_xml();
    auto body_xml = stmt_list->to_xml();
    const auto stmt_number = std::to_string(get_statement_number());

    auto opening_xml = "<" + get_node_name() + " stmt_number=\"" + stmt_number + "\">";
    auto ending_xml = "</" + get_node_name() + ">";
    return opening_xml + cond_expr_xml + body_xml + ending_xml;
}

auto WhileNode::populate_pkb_entities(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    write_facade->add_statement(std::to_string(get_statement_number()), StatementType::While);
}

auto WhileNode::populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade,
                                      std::shared_ptr<ModifyMap> modify_map) -> std::unordered_set<std::string> {
    // Modifies(s, v) for s = While
    auto stmt_number = std::to_string(get_statement_number());
    auto stmt_list_node = std::dynamic_pointer_cast<StatementListNode>(stmt_list);

    auto combined_set = std::unordered_set<std::string>();

    for (const auto& stmt_node : stmt_list_node->statements) {
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

auto WhileNode::get_stmt_nums(const std::shared_ptr<StatementListNode>& node) const -> std::unordered_set<std::string> {
    // Consider only directly nested statements (i.e. only Parent relationship). Indirectly nested statements (i.e. Parent* relationship) are handled by PKB.
    auto statement_nums = std::unordered_set<std::string>{};
    auto statements = node->statements;
    std::for_each(statements.begin(), statements.end(), [&statement_nums](const auto& statement) {
        auto statement_node = std::dynamic_pointer_cast<StatementNode>(statement);
        auto statement_num = std::to_string(statement_node->get_statement_number());
        statement_nums.insert(statement_num);
    });
    return statement_nums;
}

auto WhileNode::populate_pkb_parent(const std::shared_ptr<WriteFacade>& write_facade) const -> void {
    auto parent_statement_num = std::to_string(get_statement_number());
    auto children_statement_nums = get_stmt_nums(stmt_list);
    for (const auto& child_statement_num : children_statement_nums) {
        write_facade->add_parent(parent_statement_num, child_statement_num);
    }
}

auto WhileNode::get_vars_from_expr(const std::shared_ptr<AstNode>& expr) const -> std::unordered_set<std::string> {
    if (NodeTypeChecker::is_var_node(expr)) {
        // If expression is a variable, add to combined set.
        auto var_node = std::dynamic_pointer_cast<VarNode>(expr);
        return {var_node->name};
    }

    auto combined_set = std::unordered_set<std::string>();
    auto children = expr->get_children();
    std::for_each(children.begin(), children.end(), [&](const auto& child) { // Iterate through each child.
        auto child_var_names = get_vars_from_expr(child);
        std::for_each(child_var_names.begin(), child_var_names.end(), [&](const auto& var_name) {
            combined_set.insert(var_name); // Add each variable of a child to the combined set.
        });
    });
    return combined_set;
}

auto WhileNode::populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade, std::shared_ptr<UsesMap> uses_map) const -> std::unordered_set<std::string> {
    // Uses(s, v) holds if v appears in the condition of s
    auto stmt_number = std::to_string(get_statement_number());
    auto stmt_lsts = std::vector<std::shared_ptr<AstNode>>{cond_expr, stmt_list};
    auto combined_set = std::unordered_set<std::string>();

    // Traverse the conditional expression
    std::for_each(stmt_lsts.begin(), stmt_lsts.end(), [&](const auto& stmt_lst) {
        auto var_names = get_vars_from_expr(stmt_lst);
        std::for_each(var_names.begin(), var_names.end(), [&](const auto& var_name) {
            combined_set.insert(var_name);
        });
    });

    // Return consolidated variables
    std::for_each(combined_set.begin(), combined_set.end(), [&](const auto& var_name) {
        write_facade->add_statement_uses_var(stmt_number, var_name);
    });

    return combined_set;
}

} // namespace sp
