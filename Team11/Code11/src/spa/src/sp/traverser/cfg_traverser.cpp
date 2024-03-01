#include "sp/traverser/cfg_traverser.hpp"

namespace sp {

auto CfgTraverser::add_to_cfg(std::string stmt_num) {
    // Add stmt_num into current cfg node
    // Add stmt_num into StatementNumberMap

}

auto CfgTraverser::build_cfg(std::shared_ptr<AstNode> ast) -> void {
    if (NodeTypeChecker::is_program_node(ast)) {
        auto prog_node = std::dynamic_pointer_cast<ProgramNode>(ast);

        // initialise proc map


        auto procedures = prog_node->procedures;
        for (const auto& proc_node : procedures) {
            if (!NodeTypeChecker::is_procedure_node(proc_node)) {
                // Throw warning
                continue;
            }
            build_cfg(proc_node);
        }

    } else if (NodeTypeChecker::is_procedure_node(ast)) {
        // For proecdure node
    } else if (NodeTypeChecker::is_stmt_list_node(ast)) {
        // For stmt list node
    } else if (NodeTypeChecker::is_while_node(ast)) {

    } else if (NodeTypeChecker::is_if_node(ast)) {

    } else if (NodeTypeChecker::is_stmt_node(ast)) {
        // For stmt node
    } else {
        // Throw warning
    }

}

auto CfgTraverser::traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>&)
    -> std::shared_ptr<AstNode> {
    // Check if its a program node
    // If not, throw warning + return ast

    // Get procedure children
    // For each procedure, get the Cfg
    // Add (procedure, cfg) to the map

    // return ast

    if (!NodeTypeChecker::is_program_node(ast)) {
        // Should not start building from a non-program node
        // Throw warning
        return ast;
    }
    auto prog_node = std::dynamic_pointer_cast<ProgramNode>(ast);
    build_cfg(prog_node);
    return ast;
}
}