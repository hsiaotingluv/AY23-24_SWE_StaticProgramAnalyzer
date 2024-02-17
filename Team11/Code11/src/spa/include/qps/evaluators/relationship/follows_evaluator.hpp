#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/entities/entity_scanner.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class FollowsEvaluator : public ClauseEvaluator {
    Follows follows;

    static auto eval_follows(const std::shared_ptr<ReadFacade>& read_facade);

public:
    FollowsEvaluator(std::shared_ptr<ReadFacade> read_facade, Follows follows): ClauseEvaluator(std::move(read_facade)), follows(std::move(follows)) {
    }

    auto evaluate() -> std::optional<Table> override;

};

}