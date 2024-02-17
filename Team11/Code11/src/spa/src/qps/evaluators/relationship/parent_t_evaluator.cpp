#include "qps/evaluators/relationship/parent_t_evaluator.hpp"

namespace qps {

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded {

            [read_facade](auto& x, auto& y) -> std::optional<Table> {
                return Table{};
            }
    };
}

auto ParentTEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_parent_t(read_facade), parent_t.stmt1, parent_t.stmt2);
}
}