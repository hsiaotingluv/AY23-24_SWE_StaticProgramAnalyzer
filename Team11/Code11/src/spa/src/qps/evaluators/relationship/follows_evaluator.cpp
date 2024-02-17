#include "qps/evaluators/relationship/follows_evaluator.hpp"

namespace qps {

auto FollowsEvaluator::eval_follows(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded {

            [read_facade](auto& x, auto& y) -> std::optional<Table> {
                return Table{};
            }
    };
}

auto FollowsEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_follows(read_facade), follows.stmt1, follows.stmt2);
}
}