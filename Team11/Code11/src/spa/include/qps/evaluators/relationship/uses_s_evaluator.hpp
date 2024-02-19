#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class UsesSEvaluator : public ClauseEvaluator {
    UsesS uses_s;

    static auto eval_uses_s(const std::shared_ptr<ReadFacade>& read_facade);

  public:
    UsesSEvaluator(std::shared_ptr<ReadFacade> read_facade, UsesS uses_s)
        : ClauseEvaluator(std::move(read_facade)), uses_s(std::move(uses_s)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps