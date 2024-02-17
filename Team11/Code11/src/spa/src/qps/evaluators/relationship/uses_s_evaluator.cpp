#include "qps/evaluators/relationship/uses_s_evaluator.hpp"

namespace qps {

auto UsesSEvaluator::eval_uses_s(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded {

            [read_facade](auto& x, auto& y) -> std::optional<Table> {
                return Table{};
            }
    };
}

auto UsesSEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_uses_s(read_facade), uses_s.stmt, uses_s.ent);
}

}