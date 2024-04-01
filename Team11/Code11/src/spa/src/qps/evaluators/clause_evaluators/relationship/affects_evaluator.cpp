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
    // TODO: We should optimise this (current implementation is quite naive)
    auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};

    // Get all variables used by stmt_num_2
    auto used_vars = read_facade->get_vars_used_by_statement(stmt_num_2.value);

    // From the relevant statements, filter such that they are only assign statements and
    // the stmts modify any of the used vars (assignment statements only modify one var)
    for (const auto& stmt : relevant_stmts) {
        if (!read_facade->has_assign_statement(stmt)) {
            relevant_stmts.erase(stmt);
        } else {
            auto modified_var = *read_facade->get_vars_modified_by_statement(stmt).begin();
            if (used_vars.find(modified_var) == used_vars.end()) {
                relevant_stmts.erase(stmt);
            }
        }
    }

    // For all remaining relevant statements
    for (const auto& stmt : relevant_stmts) {
        // Get all transitive stmts from stmt_num_2
        auto next_map = read_facade->get_all_next();
        auto affect_conds = AffectsConditions(stmt, read_facade);
        auto has_transitive =
            has_transitive_rs(stmt, {stmt_num_2.value}, next_map, affect_conds.get_start_node_cond(),
                              affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

        if (has_transitive) {
            table.add_row({stmt});
        }
    }

    return table;
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
    const auto next_map = read_facade->get_all_next();

    auto affect_conds = AffectsConditions(stmt_num_1.value, read_facade);

    // Get the vars modified by stmt_num_1
    auto mod_vars = read_facade->get_vars_modified_by_statement(stmt_num_1.value);

    // If mod_vars size is not 1, return empty table
    if (mod_vars.size() != 1) {
        return Table{};
    }

    auto mod_var = *mod_vars.begin();

    // Get all stmts that use the mod_var
    auto used_stmts = read_facade->get_statements_that_use_var(mod_var);

    auto has_transitive =
        has_transitive_rs(stmt_num_1.value, used_stmts, next_map, affect_conds.get_start_node_cond(),
                          affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

    if (has_transitive) {
        return UnitTable{};
    }

    return Table{};
}

auto AffectsEvaluator::eval_affects(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    // TODO:
}

auto AffectsEvaluator::eval_affects(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    // TODO: We should optimise this (current implementation is quite naive)
    // Get all variables used by stmt_num_2
    auto used_vars = read_facade->get_vars_used_by_statement(stmt_num_2.value);

    // Get all assign statements that modify any of the used vars
    auto assign_stmts = read_facade->get_assign_statements();

    for (const auto& stmt : assign_stmts) {
        auto modified_var = *read_facade->get_vars_modified_by_statement(stmt).begin();
        if (used_vars.find(modified_var) == used_vars.end()) {
            assign_stmts.erase(stmt);
        }
    }

    // For the remaining assign_stmts
    for (const auto& stmt : assign_stmts) {
        // Get all transitive stmts from stmt_num_2
        auto next_map = read_facade->get_all_next();
        auto affect_conds = AffectsConditions(stmt, read_facade);
        auto has_transitive =
            has_transitive_rs(stmt, {stmt_num_2.value}, next_map, affect_conds.get_start_node_cond(),
                              affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

        if (has_transitive) {
            return UnitTable{};
        }
    }

    return Table{};
}

auto AffectsEvaluator::eval_affects(const WildCard&, const WildCard&) const -> OutputTable {
    // TODO:
}
} // namespace qps