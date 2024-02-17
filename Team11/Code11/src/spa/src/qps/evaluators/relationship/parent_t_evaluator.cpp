#include "qps/evaluators/relationship/parent_t_evaluator.hpp"

namespace qps {

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded {

    };
}

auto ParentTEvaluator::evaluate() -> std::optional<Table> {
    return ClauseEvaluator::evaluate();
}
}