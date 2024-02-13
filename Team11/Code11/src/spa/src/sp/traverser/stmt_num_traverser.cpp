#include "sp/traverser/stmt_num_traverser.hpp"
#include "common/ast/ast.hpp"
#include "common/ast/statement_ast.hpp"
#include <memory>

namespace sp {

auto StmtNumTraverser::is_stmt_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<StatementNode>(node) != nullptr);
}

auto StmtNumTraverser::is_read_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<ReadNode>(node) != nullptr);
}

auto StmtNumTraverser::is_call_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<CallNode>(node) != nullptr);
}

auto StmtNumTraverser::is_assign_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<AssignmentNode>(node) != nullptr);
}

auto StmtNumTraverser::is_if_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<IfNode>(node) != nullptr);
}

auto StmtNumTraverser::is_while_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<WhileNode>(node) != nullptr);
}

auto StmtNumTraverser::is_print_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<PrintNode>(node) != nullptr);
}

auto StmtNumTraverser::set_stmt_num(const std::shared_ptr<AstNode>& node, uint32_t stmt_num) -> uint32_t {
    if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<StatementNode>(node);
        stmt_node->set_statement_number(stmt_num);
        add_statement_pkb(stmt_num, stmt_node);
        stmt_num += 1;
    }

    for (const auto& child : node->get_children()) {
        stmt_num = set_stmt_num(child, stmt_num);
    }
    return stmt_num;
}

void StmtNumTraverser::add_statement_pkb(uint32_t stmt_num, const std::shared_ptr<StatementNode>& stmt_node) {
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

    write_facade->addStatement(std::to_string(stmt_num), statement_type);
}

auto StmtNumTraverser::traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> {
    auto curr_stmt_num = 1;
    set_stmt_num(node, curr_stmt_num);
    return node;
}
} // namespace sp
