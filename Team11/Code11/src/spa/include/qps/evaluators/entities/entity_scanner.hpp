#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/type_traits.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {
const auto scan_entities = [](std::shared_ptr<ReadFacade> read_facade) {
    return overloaded{
        [read_facade](const qps::VarSynonym& var) {
            return read_facade->get_variables();
        },
        [read_facade](const qps::ProcSynonym& proc) {
            return read_facade->get_procedures();
        },
        [read_facade](const qps::ConstSynonym& constant) {
            return read_facade->get_constants();
        },
        [read_facade](const qps::UntypedStmtSynonym& stmt) {
            return read_facade->get_all_statements();
        },
        [read_facade](const qps::AssignSynonym& stmt) {
            return read_facade->get_assign_statements();
        },
        [read_facade](const qps::IfSynonym& stmt) {
            return read_facade->get_if_statements();
        },
        [read_facade](const qps::WhileSynonym& stmt) {
            return read_facade->get_while_statements();
        },
        [read_facade](const qps::ReadSynonym& stmt) {
            return read_facade->get_read_statements();
        },
        [read_facade](const qps::PrintSynonym& stmt) {
            return read_facade->get_print_statements();
        },
        [read_facade](const qps::CallSynonym& stmt) {
            return read_facade->get_call_statements();
        },
        [](const auto& x) {
            return std::unordered_set<std::string>{};
        } // catch-all
    };
};
}