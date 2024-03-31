#include "qps/evaluators/clause_evaluators/relationship/affects_evaluator.hpp"
#include <stack>

namespace qps {

auto AffectsEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_affects(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

/*
 * Affects(s1, s2) holds IF
 * a1 and a2 are in the same procedure
 * a1 modifies a variable v which is used in a2
 * There is a path from a1 to a2 and v is not modified in any assignment, read, or procedure call stmt
 * (not container stmt (if/while) or procedure declaration) in that path
 */
auto AffectsEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), affects.stmt1, affects.stmt2);
}

auto AffectsEvaluator::eval_affects(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const
    -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const WildCard&, const WildCard&) const -> OutputTable {
    // TODO:
}
} // namespace qps