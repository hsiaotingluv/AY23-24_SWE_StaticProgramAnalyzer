#include "sp/traverser/design_entites_populator_traverser.hpp"

namespace sp {

auto DesignEntitiesPopulatorTraverser::traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>&)
    -> std::shared_ptr<AstNode> {
    if (NodeTypeChecker::is_const_node(node)) {
        auto const_node = std::dynamic_pointer_cast<ConstantNode>(node);
        write_facade->add_constant(std::to_string(const_node->integer));
    } else if (NodeTypeChecker::is_var_node(node)) {
        auto var_node = std::dynamic_pointer_cast<VarNode>(node);
        write_facade->add_variable(var_node->name);
    } else if (NodeTypeChecker::is_proc_node(node)) {
        auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        write_facade->add_procedure(proc_node->proc_name);
    } else if (NodeTypeChecker::is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<StatementNode>(node);
        add_statement_pkb(stmt_node->get_statement_number(), stmt_node);
    }

    for (const auto& child : node->get_children()) {
        traverse(child, {});
    }

    return node;
}

void DesignEntitiesPopulatorTraverser::add_statement_pkb(uint32_t stmt_num,
                                                         const std::shared_ptr<StatementNode>& stmt_node) {
    StatementType statement_type = StatementType::Raw;
    if (NodeTypeChecker::is_assign_node(stmt_node)) {
        statement_type = StatementType::Assign;
    } else if (NodeTypeChecker::is_call_node(stmt_node)) {
        statement_type = StatementType::Call;
    } else if (NodeTypeChecker::is_if_node(stmt_node)) {
        statement_type = StatementType::If;
    } else if (NodeTypeChecker::is_while_node(stmt_node)) {
        statement_type = StatementType::While;
    } else if (NodeTypeChecker::is_read_node(stmt_node)) {
        statement_type = StatementType::Read;
    } else if (NodeTypeChecker::is_print_node(stmt_node)) {
        statement_type = StatementType::Print;
    }

    write_facade->add_statement(std::to_string(stmt_num), statement_type);
}

} // namespace sp
