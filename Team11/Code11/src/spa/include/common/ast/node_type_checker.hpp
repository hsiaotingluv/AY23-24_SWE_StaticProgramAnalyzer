#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/binary_node_ast.hpp"
#include "common/ast/factor_ast.hpp"
#include "common/ast/logical_ast.hpp"
#include "common/ast/null_ast.hpp"
#include "common/ast/procedure_ast.hpp"
#include "common/ast/program_ast.hpp"
#include "common/ast/rel_expr_ast.hpp"
#include "common/ast/statement_ast.hpp"
#include "common/ast/statement_list_ast.hpp"

namespace sp {
/**
 * @brief NodeTypeChecker is a utility class to verify the type of an AST Node, before Type Casting (to refactor out)
 */
class NodeTypeChecker {
  public:
    /**
     * @brief Check if the given node is a Variable ASTNode.
     */
    static auto is_var_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<VarNode>(node) != nullptr;
    }

    /**
     * @brief Check if the given node is a Statement ASTNode.
     */
    static auto is_stmt_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<StatementNode>(node) != nullptr;
    }

    /**
     * @brief Check if the given node is a Program ASTNode.
     */
    static auto is_program_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<ProgramNode>(node) != nullptr;
    }
};
} // namespace sp