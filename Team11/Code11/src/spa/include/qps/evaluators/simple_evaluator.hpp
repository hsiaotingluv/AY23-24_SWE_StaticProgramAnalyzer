#pragma once

#include "PKB/Facades/ReadFacade.h"
#include "qps/parser/parser.hpp"

#include <unordered_set>

namespace qps {
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
} // namespace qps

namespace qps::stub {
// TODO: This namespace should be removed once the actual evaluators are fully implemented!
const auto get_supported_operations = [](std::shared_ptr<ReadFacade> readFacade) {
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
        [](const auto& x) {
            return std::unordered_set<std::string>{};
        } // catch-all
    };
};

inline auto evaluate(qps::Query query_obj, std::shared_ptr<ReadFacade> readFacade) -> std::vector<std::string> {
    if (!query_obj.clauses.empty()) {
        throw std::runtime_error("Not implemented");
    }
    const auto reference = query_obj.reference;
    const auto results = std::visit(get_supported_operations(readFacade), reference);
    return {results.begin(), results.end()};
};
} // namespace qps::stub
