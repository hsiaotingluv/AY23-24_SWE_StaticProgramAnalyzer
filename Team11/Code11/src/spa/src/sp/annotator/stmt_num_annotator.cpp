#include "sp/annotator/stmt_num_annotator.hpp"
#include "sp/parser/ast/ast.hpp"
#include <memory>

namespace sp {

auto StmtNumAnnotator::is_stmt_node(std::shared_ptr<AstNode> node) -> bool {
    return (dynamic_cast<StatementNode*>(node.get()) != nullptr);
}

auto StmtNumAnnotator::set_stmt_num(std::shared_ptr<AstNode> node, uint32_t stmt_num) -> uint32_t {
    if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<StatementNode>(node);
        stmt_node->set_statement_number(stmt_num);
        stmt_num += 1;
    }

    for (auto child : node->get_children()) {
        stmt_num = set_stmt_num(child, stmt_num);
    }
    return stmt_num;
}

auto StmtNumAnnotator::annotate(std::shared_ptr<AstNode> ast) -> std::shared_ptr<AstNode> {
    auto curr_stmt_num = 1;
    set_stmt_num(ast, curr_stmt_num);
    return ast;
}
} // namespace sp
