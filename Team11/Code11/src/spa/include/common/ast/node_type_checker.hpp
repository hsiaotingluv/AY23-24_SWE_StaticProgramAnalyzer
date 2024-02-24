#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/binary_node_ast.hpp"
#include "common/ast/factor_ast.hpp"
#include "common/ast/logical_ast.hpp"
#include "common/ast/null_ast.hpp"
#include "common/ast/procedure_ast.hpp"
#include "common/ast/rel_expr_ast.hpp"
#include "common/ast/statement_ast.hpp"
#include "common/ast/statement_list_ast.hpp"

namespace sp {

class NodeTypeChecker {

  public:
    static auto is_assignment_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<AssignmentNode>(node) != nullptr;
    }

    static auto is_const_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<ConstantNode>(node) != nullptr;
    }

    static auto is_var_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<VarNode>(node) != nullptr;
    }

    static auto is_proc_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<ProcedureNode>(node) != nullptr;
    }

    static auto is_stmt_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<StatementNode>(node) != nullptr;
    }

    static auto is_assign_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<AssignmentNode>(node) != nullptr;
    }

    static auto is_call_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<CallNode>(node) != nullptr;
    }

    static auto is_if_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<IfNode>(node) != nullptr;
    }

    static auto is_while_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<WhileNode>(node) != nullptr;
    }

    static auto is_read_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<ReadNode>(node) != nullptr;
    }

    static auto is_print_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<PrintNode>(node) != nullptr;
    }

    static auto is_binop_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<BinopNode>(node) != nullptr;
    }
};
} // namespace sp