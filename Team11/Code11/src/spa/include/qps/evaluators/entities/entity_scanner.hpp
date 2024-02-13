#pragma once

#include "PKB/Facades/ReadFacade.h"
#include "qps/evaluators/type_traits.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {
const auto scan_entities = [](std::shared_ptr<ReadFacade> readFacade) {
    return overloaded{
        [readFacade](const qps::VarSynonym& var) {
            return readFacade->getVariables();
        },
        [readFacade](const qps::ProcSynonym& proc) {
            return readFacade->getProcedures();
        },
        [readFacade](const qps::ConstSynonym& constant) {
            return readFacade->getConstants();
        },
        [readFacade](const qps::RawStmtSynonym& stmt) {
            return readFacade->getRawStatements();
        },
        [readFacade](const qps::AssignSynonym& stmt) {
            return readFacade->getAssignStatements();
        },
        [readFacade](const qps::IfSynonym& stmt) {
            return readFacade->getIfStatements();
        },
        [readFacade](const qps::WhileSynonym& stmt) {
            return readFacade->getWhileStatements();
        },
        [readFacade](const qps::ReadSynonym& stmt) {
            return readFacade->getReadStatements();
        },
        [readFacade](const qps::PrintSynonym& stmt) {
            return readFacade->getPrintStatements();
        },
        [readFacade](const qps::CallSynonym& stmt) {
            return readFacade->getCallStatements();
        },
        [](const auto& x) {
            return std::unordered_set<std::string>{};
        } // catch-all
    };
};
}