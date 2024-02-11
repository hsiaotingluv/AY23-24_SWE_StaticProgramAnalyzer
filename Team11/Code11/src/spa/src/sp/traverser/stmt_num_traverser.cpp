#include "sp/traverser/stmt_num_traverser.hpp"
#include "sp/parser/ast/ast.hpp"
#include "sp/parser/ast/statement_ast.hpp"
#include <memory>

namespace sp {

auto StmtNumTraverser::is_stmt_node(const std::shared_ptr<AstNode>& node) -> bool {
    return (std::dynamic_pointer_cast<StatementNode>(node) != nullptr);
}

auto StmtNumTraverser::set_stmt_num(const std::shared_ptr<AstNode>& node, uint32_t stmt_num) -> uint32_t {
    if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<StatementNode>(node);
        stmt_node->set_statement_number(stmt_num);
        stmt_num += 1;
    }

    for (const auto& child : node->get_children()) {
        stmt_num = set_stmt_num(child, stmt_num);
    }
    return stmt_num;
}

auto StmtNumTraverser::traverse(std::shared_ptr<AstNode> node) -> std::shared_ptr<AstNode> {
    auto curr_stmt_num = 1;
    set_stmt_num(node, curr_stmt_num);
    return node;
}
} // namespace sp
