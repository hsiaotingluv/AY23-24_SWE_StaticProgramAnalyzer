#include "qps/evaluators/relationship/next_t_evaluator.hpp"

namespace qps {

auto NextTEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_next_t(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto NextTEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), next_t.stmt1, next_t.stmt2);
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                 const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    // TODO:
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> OutputTable {
    // TODO:
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const -> OutputTable {
    // TODO:
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    // TODO:
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    // TODO:
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    // TODO:
}

auto NextTEvaluator::eval_next_t(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    // TODO:
}

auto NextTEvaluator::eval_next_t(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    // TODO:
}

auto NextTEvaluator::eval_next_t(const WildCard&, const WildCard&) const -> OutputTable {
    // TODO:
}
} // namespace qps