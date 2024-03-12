#pragma once

#include "common/ast/ast.hpp"
#include "common/cfg/cfg.hpp"
#include "sp/traverser/traverser.hpp"

namespace sp {

using ProcMap = std::unordered_map<std::string, std::shared_ptr<Cfg>>;        // Map of Procedure Name -> Cfg.
using StmtNumMap = std::unordered_map<std::string, std::shared_ptr<CfgNode>>; // Map of Statement Number -> CfgNode.

class CfgBuilder {
  private:
    ProcMap proc_map{};        // Only 1 Proc Map for the entire Program.
    StmtNumMap stmt_num_map{}; // Only 1 Stmt Num Map for the entire Program.

    /**
     * @brief Initialise an empty Cfg for the given Procedure Name.
     */
    auto create_empty_cfg(std::string proc_name) -> std::shared_ptr<Cfg>;

    /**
     * @brief Populate the StmtNumMap within a specific Node.
     */
    auto populate_stmt_num_map_by_node(StatementNumbers stmt_nums, std::shared_ptr<CfgNode> node) -> StatementNumbers;

    /**
     * @brief Populate the StmtNumMap within a specific procedure.
     */
    auto populate_stmt_num_map_by_procedure(Graph graph) -> Graph;

    /**
     * @brief Populate the StmtNumMap after the CFG is built.
     */
    auto populate_stmt_num_map() -> StmtNumMap;

  public:
    /**
     * @brief Get the ProcMap.
     */
    auto get_proc_map() const -> ProcMap;

    /**
     * @brief Get the StmtNumMap.
     */
    auto get_stmt_num_map() const -> StmtNumMap;

    /**
     * @brief Build a Control Flow Graph for the given Program ASTNode.
     */
    auto build(std::shared_ptr<AstNode> ast) -> ProcMap;

    /**
     * @brief Construct a string representation of the Control Flow Graph.
     */
    friend auto operator<<(std::ostream& os, const CfgBuilder& cfg_builder) -> std::ostream&;
};
} // namespace sp