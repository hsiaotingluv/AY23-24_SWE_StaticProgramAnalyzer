#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/node_type_checker.hpp"
#include "common/cfg/cfg.hpp"
#include "sp/traverser/traverser.hpp"

namespace sp {

using ProcMap = std::unordered_map<std::string, std::shared_ptr<Cfg>>; // Map of Procedure Statement Number -> Cfg.

class CfgBuilder {
  private:
    /**
     * @brief Initialise an empty Cfg for the given Procedure Name.
     */
    auto create_empty_cfg(std::string proc_name) -> std::shared_ptr<Cfg> {
        auto cfg = std::make_shared<Cfg>();
        cfg->add_node_to_graph();                        // Add the first node.
        proc_map.insert(std::make_pair(proc_name, cfg)); // Add the Cfg to the ProcMap.
        return cfg;
    };

  public:
    ProcMap proc_map{}; // Public for Testing Purpose

    /**
     * @brief Build a Control Flow Graph for the given Program ASTNode.
     */
    auto build(std::shared_ptr<AstNode> ast) -> ProcMap {
        if (!NodeTypeChecker::is_program_node(ast)) {
            return proc_map; // Invalid Node -> No valid ProcMap.
        }

        auto prog_node = std::dynamic_pointer_cast<ProgramNode>(ast);
        for (const auto& node : prog_node->procedures) {
            auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
            auto proc_name = proc_node->proc_name;
            auto stmt_list = proc_node->stmt_list;

            auto cfg = create_empty_cfg(proc_name);
            stmt_list->build_cfg(cfg);
        }

        return proc_map;
    }

    /**
     * @brief Construct a string representation of the Control Flow Graph.
     */
    auto to_string() -> std::string {
        std::string result = "";
        for (const auto& [proc_name, cfg] : proc_map) {
            result += proc_name + ":\n";
            result += cfg->to_string();
        }
        return result;
    };
};
} // namespace sp