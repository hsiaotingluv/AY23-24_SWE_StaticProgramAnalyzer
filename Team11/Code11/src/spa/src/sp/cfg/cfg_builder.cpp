#pragma once

#include "sp/cfg/cfg_builder.hpp"

namespace sp {

auto CfgBuilder::create_empty_cfg(std::string proc_name) -> std::shared_ptr<Cfg> {
    auto cfg = std::make_shared<Cfg>();
    proc_map.insert(std::make_pair(proc_name, cfg)); // Add the Cfg to the ProcMap.
    return cfg;
};

auto CfgBuilder::get_proc_map() const -> ProcMap {
    return proc_map;
};

auto CfgBuilder::build(std::shared_ptr<AstNode> ast) -> ProcMap {
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

auto operator<<(std::ostream& os, const CfgBuilder& cfg_builder) -> std::ostream& {
    auto proc_map = cfg_builder.proc_map;
    for (const auto& [proc_name, cfg] : proc_map) {
        os << proc_name << ":\n" << *cfg;
    }
    return os;
}
} // namespace sp