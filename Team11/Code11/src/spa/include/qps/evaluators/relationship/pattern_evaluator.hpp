#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"

namespace qps {
class PatternEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    PatternClause pattern;

    static auto eval_pattern(const std::shared_ptr<ReadFacade>& read_facade,
                             const std::shared_ptr<AssignSynonym>& assign_synonym);

  public:
    PatternEvaluator(std::shared_ptr<ReadFacade> read_facade, PatternClause pattern)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), pattern(std::move(pattern)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps