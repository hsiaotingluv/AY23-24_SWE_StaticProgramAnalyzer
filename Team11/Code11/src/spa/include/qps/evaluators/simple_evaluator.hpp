#pragma once

#include "PKB/Facades/ReadFacade.h"
#include "qps/evaluators/results_map.hpp"
#include "qps/parser/parser.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace qps {
class Evaluator {
    ResultsMap results_map;
    std::shared_ptr<ReadFacade> readFacade;

  public:
    Evaluator(std::shared_ptr<ReadFacade> readFacade) : readFacade(std::move(readFacade)) {
    }

    auto evaluate(qps::Query query_obj) -> std::vector<std::string>;
};

} // namespace qps
