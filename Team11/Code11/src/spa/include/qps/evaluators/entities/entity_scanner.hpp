#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"
#include <memory>

namespace qps {
auto scan_entities(std::shared_ptr<ReadFacade> read_facade) {
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
        [read_facade](const qps::AnyStmtSynonym&) {
            return read_facade->get_all_statements();
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

auto scan_entities(std::shared_ptr<ReadFacade> read_facade, const std::shared_ptr<Synonym>& synonym) {
    if (const auto maybe_var = std::dynamic_pointer_cast<VarSynonym>(synonym)) {
        return read_facade->get_variables();
    }
    if (const auto maybe_proc = std::dynamic_pointer_cast<ProcSynonym>(synonym)) {
        return read_facade->get_procedures();
    }
    if (const auto maybe_const = std::dynamic_pointer_cast<ConstSynonym>(synonym)) {
        return read_facade->get_constants();
    }
    if (const auto maybe_stmt = std::dynamic_pointer_cast<AnyStmtSynonym>(synonym)) {
        return read_facade->get_all_statements();
    }
    if (const auto maybe_assign = std::dynamic_pointer_cast<AssignSynonym>(synonym)) {
        return read_facade->get_assign_statements();
    }
    if (const auto maybe_if = std::dynamic_pointer_cast<IfSynonym>(synonym)) {
        return read_facade->get_if_statements();
    }
    if (const auto maybe_while = std::dynamic_pointer_cast<WhileSynonym>(synonym)) {
        return read_facade->get_while_statements();
    }
    if (const auto maybe_read = std::dynamic_pointer_cast<ReadSynonym>(synonym)) {
        return read_facade->get_read_statements();
    }
    if (const auto maybe_print = std::dynamic_pointer_cast<PrintSynonym>(synonym)) {
        return read_facade->get_print_statements();
    }
    if (const auto maybe_call = std::dynamic_pointer_cast<CallSynonym>(synonym)) {
        return read_facade->get_call_statements();
    }
    throw std::runtime_error("Unknown synonym type");
}

} // namespace qps