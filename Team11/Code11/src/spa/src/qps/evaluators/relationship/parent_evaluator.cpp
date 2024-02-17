#include "qps/evaluators/relationship/parent_evaluator.hpp"

namespace qps {

auto ParentEvaluator::eval_parent(const std::shared_ptr<ReadFacade>& read_facade) {
    return nullptr;
}

auto ParentEvaluator::evaluate() -> std::optional<Table> {

}
}