#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"

#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class FollowsTEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    FollowsT follows_t;

    static auto eval_follows_t(const std::shared_ptr<ReadFacade>& read_facade);

  public:
    FollowsTEvaluator(std::shared_ptr<ReadFacade> read_facade, FollowsT follows_t)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), follows_t(std::move(follows_t)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps