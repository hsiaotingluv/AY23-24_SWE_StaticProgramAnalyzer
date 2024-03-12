#pragma once

#include "common/ast/ast.hpp"
#include "common/cfg/cfg.hpp"
#include "sp/traverser/traverser.hpp"

namespace sp {

using ProcMap = std::unordered_map<std::string, std::shared_ptr<Cfg>>; // Map of Procedure Statement Number -> Cfg.

class CfgBuilder {
  private:
    ProcMap proc_map{}; // Only 1 Proc Map for the entire Program.

    /**
     * @brief Initialise an empty Cfg for the given Procedure Name.
     */
    auto create_empty_cfg(std::string proc_name) -> std::shared_ptr<Cfg>;

  public:
    /**
     * @brief Get the ProcMap.
     */
    auto get_proc_map() const -> ProcMap;

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