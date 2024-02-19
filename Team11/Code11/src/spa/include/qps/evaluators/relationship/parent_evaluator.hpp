#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class ParentEvaluator : public ClauseEvaluator {
    Parent parent;

    static auto eval_parent(const std::shared_ptr<ReadFacade>& read_facade);

  public:
    ParentEvaluator(std::shared_ptr<ReadFacade> read_facade, Parent parent)
        : ClauseEvaluator(std::move(read_facade)), parent(std::move(parent)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps