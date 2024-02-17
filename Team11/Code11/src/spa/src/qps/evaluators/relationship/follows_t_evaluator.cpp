#include "qps/evaluators/relationship/follows_t_evaluator.hpp"

namespace qps {

auto FollowsTEvaluator::eval_follows_t(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded {

            [read_facade](auto& x, auto& y) -> std::optional<Table> {
                return Table{};
            }
    };
}

auto FollowsTEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_follows_t(read_facade), follows_t.stmt1, follows_t.stmt2);
}
}