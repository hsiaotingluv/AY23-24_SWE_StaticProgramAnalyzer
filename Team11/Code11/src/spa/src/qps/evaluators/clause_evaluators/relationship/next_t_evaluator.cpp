#include "qps/evaluators/clause_evaluators/relationship/next_t_evaluator.hpp"
#include "qps/utils/algo.h"
#include <stack>

namespace qps {

auto NextTEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_next_t(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto NextTEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), next_t.stmt1, next_t.stmt2);
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_1);
    auto table = Table{{stmt_syn_1}};
    const auto next_reverse_map = read_facade->get_all_next_reverse();

    auto new_rows = get_all_transitive_from_node(stmt_num_2.value, next_reverse_map);

    for (const auto& row : new_rows) {
        if (relevant_stmts.find(row) != relevant_stmts.end()) {
            table.add_row({row});
        }
    }

    return table;
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_1);

    auto table = Table{{stmt_syn_1}};
    auto all_stmts = read_facade->get_all_next_keys();

    for (const auto& stmt : all_stmts) {
        if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
            table.add_row({stmt});
        }
    }

    return table;
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_2);
    auto table = Table{{stmt_syn_2}};

    const auto next_map = read_facade->get_all_next();

    auto new_rows = get_all_transitive_from_node(stmt_num_1.value, next_map);

    for (const auto& row : new_rows) {
        if (relevant_stmts.find(row) != relevant_stmts.end()) {
            table.add_row({row});
        }
    }

    return table;
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                 const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    const auto next_map = read_facade->get_all_next();

    auto next_star_pairs = get_next_star_pairs(next_map);

    if (stmt_syn_1 == stmt_syn_2) {
        Table table{{stmt_syn_1}};

        const auto relevant_stmts = get_data(stmt_syn_1);

        for (const auto& stmt : relevant_stmts) {
            if (next_star_pairs.find({stmt, stmt}) != next_star_pairs.end()) {
                table.add_row({stmt});
            }
        }
        return table;
    }

    const auto relevant_stmts_1 = get_data(stmt_syn_1);
    const auto relevant_stmts_2 = get_data(stmt_syn_2);
    Table table{{stmt_syn_1, stmt_syn_2}};

    for (const auto& stmt1 : relevant_stmts_1) {
        for (const auto& stmt2 : relevant_stmts_2) {
            if (next_star_pairs.find({stmt1, stmt2}) != next_star_pairs.end()) {
                table.add_row({stmt1, stmt2});
            }
        }
    }

    return table;
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    const auto next_map = read_facade->get_all_next();

    bool has_transitive = has_transitive_rs(stmt_num_1.value, {stmt_num_2.value}, next_map);

    if (has_transitive) {
        return UnitTable{};
    }

    return Table{};
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    bool has_next_rs = read_facade->contains_next_key(stmt_num_1.value);

    if (has_next_rs) {
        return UnitTable{};
    }

    return Table{};
}

auto NextTEvaluator::eval_next_t(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_2);

    auto table = Table{{stmt_syn_2}};
    auto all_stmts = read_facade->get_all_next_values();

    for (const auto& stmt : all_stmts) {
        if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
            table.add_row({stmt});
        }
    }

    return table;
}

auto NextTEvaluator::eval_next_t(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    bool has_next_rs = read_facade->contains_next_value(stmt_num_2.value);

    if (has_next_rs) {
        return UnitTable{};
    }

    return Table{};
}

auto NextTEvaluator::eval_next_t(const WildCard&, const WildCard&) const -> OutputTable {
    if (read_facade->has_next_relation()) {
        return UnitTable{};
    }

    return Table{};
}
} // namespace qps