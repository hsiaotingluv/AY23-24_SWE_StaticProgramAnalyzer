#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/type_traits.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {
const auto scan_entities = [](std::shared_ptr<ReadFacade> read_facade) {
    return overloaded{
        [read_facade](const qps::VarSynonym&) {
            return read_facade->get_variables();
        },
        [read_facade](const qps::ProcSynonym&) {
            return read_facade->get_procedures();
        },
        [read_facade](const qps::ConstSynonym&) {
            return read_facade->get_constants();
        },
        [read_facade](const qps::RawStmtSynonym&) {
            return read_facade->get_raw_statements();
        },
        [read_facade](const qps::AssignSynonym&) {
            return read_facade->get_assign_statements();
        },
        [read_facade](const qps::IfSynonym&) {
            return read_facade->get_if_statements();
        },
        [read_facade](const qps::WhileSynonym&) {
            return read_facade->get_while_statements();
        },
        [read_facade](const qps::ReadSynonym&) {
            return read_facade->get_read_statements();
        },
        [read_facade](const qps::PrintSynonym&) {
            return read_facade->get_print_statements();
        },
        [read_facade](const qps::CallSynonym&) {
            return read_facade->get_call_statements();
        },
        [](const auto&) {
            return std::unordered_set<std::string>{};
        } // catch-all
    };
};
}