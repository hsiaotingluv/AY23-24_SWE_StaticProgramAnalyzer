#include "common/ast/factor_ast.hpp"
#include "common/ast/procedure_ast.hpp"
#include "common/ast/statement_ast.hpp"
#include "sp/traverser/design_entites_populator_traverser.hpp"
#include <memory>

namespace sp {

auto DesignEntitiesPopulatorTraverser::is_const_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<ConstantNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_var_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<VarNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_proc_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<ProcedureNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_stmt_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<StatementNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_read_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<ReadNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_call_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<CallNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_assign_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<AssignmentNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_if_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<IfNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_while_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<WhileNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::is_print_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<PrintNode>(node) != nullptr);
}

auto DesignEntitiesPopulatorTraverser::traverse(std::shared_ptr<AstNode> node,
                                                const std::vector<std::string>& topo_sort) -> std::shared_ptr<AstNode> {
    if (is_const_node(node)) {
        auto const_node = std::dynamic_pointer_cast<ConstantNode>(node);
        write_facade->add_constant(std::to_string(const_node->integer));
    } else if (is_var_node(node)) {
        auto var_node = std::dynamic_pointer_cast<VarNode>(node);
        write_facade->add_variable(var_node->name);
    } else if (is_proc_node(node)) {
        auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        write_facade->add_procedure(proc_node->proc_name);
    } else if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<StatementNode>(node);
        add_statement_pkb(stmt_node->get_statement_number(), stmt_node);
    }

    for (const auto& child : node->get_children()) {
        traverse(child, topo_sort);
    }

    return node;
}

void DesignEntitiesPopulatorTraverser::add_statement_pkb(uint32_t stmt_num,
                                                         const std::shared_ptr<StatementNode>& stmt_node) {
    StatementType statement_type = StatementType::Raw;
    if (is_assign_node(stmt_node)) {
        statement_type = StatementType::Assign;
    } else if (is_call_node(stmt_node)) {
        statement_type = StatementType::Call;
    } else if (is_if_node(stmt_node)) {
        statement_type = StatementType::If;
    } else if (is_while_node(stmt_node)) {
        statement_type = StatementType::While;
    } else if (is_read_node(stmt_node)) {
        statement_type = StatementType::Read;
    } else if (is_print_node(stmt_node)) {
        statement_type = StatementType::Print;
    }

    write_facade->add_statement(std::to_string(stmt_num), statement_type);
}

} // namespace sp
