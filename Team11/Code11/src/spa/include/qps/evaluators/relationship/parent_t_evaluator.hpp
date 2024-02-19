#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class ParentTEvaluator : public ClauseEvaluator {
    ParentT parent_t;

    static auto eval_parent_t(const std::shared_ptr<ReadFacade>& read_facade);

  public:
    ParentTEvaluator(std::shared_ptr<ReadFacade> read_facade, ParentT parent_t)
        : ClauseEvaluator(std::move(read_facade)), parent_t(std::move(parent_t)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps