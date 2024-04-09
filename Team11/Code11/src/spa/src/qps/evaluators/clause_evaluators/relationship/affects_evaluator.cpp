#include "qps/evaluators/clause_evaluators/relationship/affects_evaluator.hpp"
#include "qps/utils/affects_conditions.h"
#include "qps/utils/algo.h"

namespace qps {

auto AffectsEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_affects(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto AffectsEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), affects.stmt1, affects.stmt2);
}

auto AffectsEvaluator::eval_affects(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> OutputTable {
    // TODO: We should optimise this (current implementation is quite naive)
    auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};

    // Get all variables used by stmt_num_2
    auto used_vars = read_facade->get_vars_used_by_statement(stmt_num_2.value);

    std::unordered_set<std::string> filtered_stmts;

    // From the relevant statements, filter such that they are only assign statements and
    // the stmts modify any of the used vars (assignment statements only modify one var)
    for (const auto& stmt : relevant_stmts) {
        if (!read_facade->has_assign_statement(stmt)) {
            continue;
        }

        auto modified_var = *read_facade->get_vars_modified_by_statement(stmt).begin();
        if (used_vars.find(modified_var) != used_vars.end()) {
            filtered_stmts.insert(stmt);
        }
    }

    auto next_map = read_facade->get_all_next();

    // For all filtered statements
    for (const auto& stmt : filtered_stmts) {
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
    // TODO: Possibly optimise?
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto next_map = read_facade->get_all_next();

    // get all statements
    auto all_stmts = read_facade->get_all_statements();

    auto table = Table{{stmt_syn_1}};

    // Get only the relevant_stmts that have transitive relationship with any other stmt
    for (const auto& stmt : relevant_stmts) {
        auto affect_conds = AffectsConditions(stmt, read_facade);

        auto has_transitive =
            has_transitive_rs(stmt, all_stmts, next_map, affect_conds.get_start_node_cond(),
                              affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

        if (has_transitive) {
            table.add_row({stmt});
        }
    }

    return table;
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
    return table;
}

auto AffectsEvaluator::eval_affects(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    // TODO: Possibly optimise
    auto next_map = read_facade->get_all_next();
    auto relevant_stmts_1 = stmt_syn_1->scan(read_facade);

    if (stmt_syn_1 == stmt_syn_2) {
        Table table{{stmt_syn_1}};

        for (const auto& stmt : relevant_stmts_1) {
            auto affect_conds = AffectsConditions(stmt, read_facade);
            auto has_transitive =
                has_transitive_rs(stmt, {stmt}, next_map, affect_conds.get_start_node_cond(),
                                  affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

            if (has_transitive) {
                table.add_row({stmt});
            }
        }
        return table;
    }

    auto relevant_stmts_2 = stmt_syn_2->scan(read_facade);

    auto table = Table{{stmt_syn_1, stmt_syn_2}};

    for (const auto& stmt_1 : relevant_stmts_1) {
        auto affect_conds = AffectsConditions(stmt_1, read_facade);

        // use get_all_transitive_from_node
        auto new_rows =
            get_all_transitive_from_node(stmt_1, next_map, affect_conds.get_start_node_cond(),
                                         affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

        // for each new row
        for (const auto& row : new_rows) {
            if (relevant_stmts_2.find(row) != relevant_stmts_2.end()) {
                table.add_row({stmt_1, row});
            }
        }
    }

    return table;
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
    // TODO: We should optimise this (current implementation is quite naive)
    auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto next_map = read_facade->get_all_next();

    auto table = Table({stmt_syn_2});

    // For each stmt in relevant_stmts
    for (const auto& stmt : relevant_stmts) {
        // Get all variables used by stmt
        auto used_vars = read_facade->get_vars_used_by_statement(stmt);

        // Get all assign statements that modify any of the used vars
        auto assign_stmts = read_facade->get_assign_statements();

        std::unordered_set<std::string> filtered_stmts;

        for (const auto& assign_stmt : assign_stmts) {
            auto modified_var = *read_facade->get_vars_modified_by_statement(assign_stmt).begin();
            if (used_vars.find(modified_var) != used_vars.end()) {
                filtered_stmts.insert(assign_stmt);
            }
        }

        // For each filtered_stmt
        for (const auto& filtered_stmt : filtered_stmts) {
            // Get all transitive stmts from stmt
            auto affect_conds = AffectsConditions(filtered_stmt, read_facade);
            auto has_transitive =
                has_transitive_rs(filtered_stmt, {stmt}, next_map, affect_conds.get_start_node_cond(),
                                  affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

            if (has_transitive) {
                table.add_row({stmt});
            }
        }
    }

    return table;
}

auto AffectsEvaluator::eval_affects(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    // TODO: We should optimise this (current implementation is quite naive)
    // Get all variables used by stmt_num_2
    auto used_vars = read_facade->get_vars_used_by_statement(stmt_num_2.value);

    // Get all assign statements that modify any of the used vars
    auto assign_stmts = read_facade->get_assign_statements();

    auto filtered_stmts = std::unordered_set<std::string>{};

    for (const auto& stmt : assign_stmts) {
        auto modified_var = *read_facade->get_vars_modified_by_statement(stmt).begin();
        if (used_vars.find(modified_var) != used_vars.end()) {
            filtered_stmts.insert(stmt);
        }
    }
    auto next_map = read_facade->get_all_next();

    // For each filtered_stmt
    for (const auto& stmt : filtered_stmts) {
        // Get all transitive stmts from stmt_num_2
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
    // TODO: Possibly optimise?
    const auto relevant_stmts = read_facade->get_assign_statements();
    auto next_map = read_facade->get_all_next();

    // get all statements
    auto all_stmts = read_facade->get_all_statements();

    // Get only the relevant_stmts that have transitive relationship with any other stmt
    for (const auto& stmt : relevant_stmts) {
        auto affect_conds = AffectsConditions(stmt, read_facade);

        auto has_transitive =
            has_transitive_rs(stmt, all_stmts, next_map, affect_conds.get_start_node_cond(),
                              affect_conds.get_end_node_cond(), affect_conds.get_intermediate_node_cond());

        if (has_transitive) {
            return UnitTable{};
        }
    }

    return Table{};
}
} // namespace qps