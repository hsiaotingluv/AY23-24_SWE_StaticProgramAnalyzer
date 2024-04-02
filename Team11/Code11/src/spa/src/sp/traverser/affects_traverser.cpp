#include "sp/traverser/affects_traverser.hpp"

namespace sp {

/**
 * @brief Get the Statement Numbers for each Procedure.
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
 * @brief Send the Statement Numbers for each Procedure to PKB.
 */
auto AffectsTraverser::send_to_pkb(const std::string proc_name, const StatementNumbers& stmt_nos) -> StatementNumbers {
    for (const auto& stmt_no : stmt_nos) {
        auto stmt_no_string = std::to_string(stmt_no);
        write_facade->add_proc_to_stmt_no_mapping(proc_name, stmt_no_string);
    }
    return stmt_nos;
}

/**
 * @brief Extract the Statement Numbers for each Procedure in the Program (for Affects relationship)
 * @note Returns a mapping of Procedure Name to Statement Numbers for testing.
 */
auto AffectsTraverser::traverse(const ProcMap& cfgs) -> std::unordered_map<std::string, StatementNumbers> {
    std::unordered_map<std::string, StatementNumbers> proc_stmt_nos;
    for (const auto& [proc_name, cfg] : cfgs) {
        auto statement_numbers = get_statement_numbers(cfg);
        send_to_pkb(proc_name, statement_numbers);
        proc_stmt_nos.insert({proc_name, statement_numbers});
    }
    return proc_stmt_nos;
}
} // namespace sp
