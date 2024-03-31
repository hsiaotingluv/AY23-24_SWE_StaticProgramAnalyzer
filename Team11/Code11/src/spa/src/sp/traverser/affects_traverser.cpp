#include "sp/traverser/affects_traverser.hpp"

namespace sp {

/**
 * @brief Extract the Affect Relationships for the Program.
 * @note ProcMap is an alias for std::unordered_map<std::string, std::shared_ptr<ProcedureCfg>>
 */
auto AffectsTraverser::get_statement_numbers(const std::shared_ptr<ProcedureCfg>& cfg) -> StatementNumbers {
    auto graph = cfg->get_graph();
    StatementNumbers stmt_nos;
    for (const auto& [node, _] : graph) { // Traverse all nodes (key) of the graph will suffice.
        auto stmts = node->get();
        for (const auto& stmt : stmts) {
            stmt_nos.push_back(stmt);
        }
    }
    return stmt_nos;
}

/**
 * @brief Extract the Affect Relationships for the Program.
 * @note ProcMap is an alias for std::unordered_map<std::string, std::shared_ptr<ProcedureCfg>>
 */
auto AffectsTraverser::send_to_pkb(const std::string proc_name, const StatementNumbers& stmt_nos) -> void {
    for (const auto& stmt_no : stmt_nos) {
        auto stmt_no_string = std::to_string(stmt_no);
        write_facade->add_proc_to_stmt_no_mapping(proc_name, stmt_no_string);
    }
}

/**
 * @brief Extract the Affect Relationships for the Program.
 * @note ProcMap is an alias for std::unordered_map<std::string, std::shared_ptr<ProcedureCfg>>
 */
auto AffectsTraverser::traverse(const ProcMap& cfgs) -> void {
    for (const auto& [proc_name, cfg] : cfgs) {
        auto statement_numbers = get_statement_numbers(cfg);
        send_to_pkb(proc_name, statement_numbers);
    }
}
} // namespace sp
