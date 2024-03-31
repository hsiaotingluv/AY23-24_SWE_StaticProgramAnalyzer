#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace qps {
class QueryEvaluator {
    std::shared_ptr<ClauseEvaluator> evaluator;
    std::shared_ptr<pkb::ReadFacade> read_facade;

  public:
    QueryEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade) : read_facade(std::move(read_facade)) {
    }

    auto evaluate(const qps::Query& query_obj) -> std::vector<std::string>;
};

} // namespace qps
