#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/results_map.hpp"
#include "qps/qps.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace qps {
class Evaluator {
    ResultsMap results_map;
    std::shared_ptr<ReadFacade> read_facade;

  public:
    Evaluator(std::shared_ptr<ReadFacade> read_facade) : read_facade(std::move(read_facade)) {
    }

    auto evaluate(qps::Query query_obj) -> std::vector<std::string>;
};

} // namespace qps
