#include "qps/evaluators/relationship/parent_evaluator.hpp"

namespace qps {

auto ParentEvaluator::eval_parent(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded {

        [read_facade](auto& x, auto& y) -> std::optional<Table> {
            return Table{};
        }
    };
}

auto ParentEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_parent(read_facade), parent.stmt1, parent.stmt2);
}
}