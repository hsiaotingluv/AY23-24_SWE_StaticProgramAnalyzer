#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

#include <memory>
#include <optional>
#include <utility>

namespace qps {

class ModifiesSEvaluator : public ClauseEvaluator {
    ModifiesS modifies_s;

    static auto eval_modifies(const std::shared_ptr<ReadFacade>& read_facade);

  public:
    ModifiesSEvaluator(std::shared_ptr<ReadFacade> read_facade, ModifiesS modifies_s)
        : ClauseEvaluator(std::move(read_facade)), modifies_s(std::move(modifies_s)) {
    }

    auto evaluate() -> std::optional<Table> override;
};
} // namespace qps
