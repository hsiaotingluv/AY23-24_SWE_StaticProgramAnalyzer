#include "sp/cfg/cfg_builder.hpp"
#include "common/ast/procedure_ast.hpp"
#include "common/ast/program_ast.hpp"

namespace sp {

auto CfgBuilder::create_empty_cfg(const std::string& proc_name) -> std::shared_ptr<Cfg> {
    auto cfg = std::make_shared<Cfg>();
    proc_map.insert(std::make_pair(proc_name, cfg)); // Add the Cfg to the ProcMap.
    return cfg;
};

auto CfgBuilder::populate_stmt_num_map_by_node(const StatementNumbers& stmt_nums, const std::string& proc_name,
                                               const std::shared_ptr<CfgNode>& node) -> StatementNumbers {
    for (const auto& stmt_num : stmt_nums) { // Iterate through each statement number.
        auto stmt_num_str = std::to_string(stmt_num);
        auto node_info = std::make_pair(proc_name, node);
        stmt_num_map.insert(std::make_pair(stmt_num_str, node_info));
    }
    return stmt_nums;
}

auto CfgBuilder::populate_stmt_num_map_by_procedure(const std::string& proc_name, const Graph& graph) -> Graph {
    for (const auto& [node, out_neighbours] : graph) { // Iterate through each CfgNode.
        auto stmt_nums = node->get();
        populate_stmt_num_map_by_node(stmt_nums, proc_name, node);
    }
    return graph;
}

auto CfgBuilder::populate_stmt_num_map() -> StmtNumMap {
    for (const auto& [proc_name, cfg] : proc_map) { // Iterate through each procedure's Cfg.
        auto graph = cfg->get_graph();
        populate_stmt_num_map_by_procedure(proc_name, graph);
    }
    return stmt_num_map;
};

auto CfgBuilder::get_proc_map() const -> ProcMap {
    return proc_map;
};

auto CfgBuilder::get_stmt_num_map() const -> StmtNumMap {
    return stmt_num_map;
};

auto CfgBuilder::build(const std::shared_ptr<AstNode>& ast) -> ProcMap {
    auto prog_node = std::dynamic_pointer_cast<ProgramNode>(ast);
    for (const auto& node : prog_node->procedures) {
        auto proc_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        auto proc_name = proc_node->proc_name;
        auto stmt_list = proc_node->stmt_list;

        auto cfg = create_empty_cfg(proc_name);
        stmt_list->build_cfg(cfg);
    }

    populate_stmt_num_map(); // Additional step to populate the StmtNumMap after the CFG is built.

    return proc_map;
}

auto operator<<(std::ostream& os, const CfgBuilder& cfg_builder) -> std::ostream& {
    auto proc_map = cfg_builder.proc_map;
    for (const auto& [proc_name, cfg] : proc_map) {
        os << proc_name << ":\n" << *cfg;
    }
    return os;
}
} // namespace sp