#include "qps/evaluators/clause_evaluators/relationship/affects_evaluator.hpp"
#include "qps/utils/affects_conditions.h"
#include "qps/utils/algo.h"

#include <stack>

namespace qps {

auto AffectsEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_affects(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

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
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table{{stmt_syn_2}};
    const auto next_map = read_facade->get_all_next();

    auto affect_conds = AffectsConditions(stmt_num_1.value, read_facade);

    auto new_rows =
        get_all_transitive_from_node(stmt_num_1.value, next_map, affect_conds.get_start_node_cond(),
                                     affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

    for (const auto& row : new_rows) {
        if (relevant_stmts.find(row) != relevant_stmts.end()) {
            table.add_row({row});
        }
    }
}

auto AffectsEvaluator::eval_affects(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    const auto next_map = read_facade->get_all_next();

    auto affect_conds = AffectsConditions(stmt_num_1.value, read_facade);

    auto has_transitive =
        has_transitive_rs(stmt_num_1.value, {stmt_num_2.value}, next_map, affect_conds.get_start_node_cond(),
                          affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

    if (has_transitive) {
        return UnitTable{};
    }

    return Table{};
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