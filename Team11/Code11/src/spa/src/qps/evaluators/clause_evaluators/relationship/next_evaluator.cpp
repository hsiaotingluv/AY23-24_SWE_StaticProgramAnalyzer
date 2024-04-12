#include "qps/evaluators/clause_evaluators/relationship/next_evaluator.hpp"

namespace qps {

auto NextEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_next(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto NextEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), next.stmt1, next.stmt2);
}

auto NextEvaluator::eval_next(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                              const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {

    if (stmt_syn_1 == stmt_syn_2) {
        return Table{};
    }

    const auto relevant_stmts_1 = get_data(stmt_syn_1);
    const auto relevant_stmts_2 = get_data(stmt_syn_2);

    auto table = Table{{stmt_syn_1, stmt_syn_2}};
    const auto all_next_keys = read_facade->get_all_next_keys();
    for (const auto& next_key : all_next_keys) {
        if (relevant_stmts_1.find(next_key) == relevant_stmts_1.end()) {
            continue;
        }

        const auto all_values_of_key = read_facade->get_next_of(next_key);
        for (const auto& next_value : all_values_of_key) {
            if (relevant_stmts_2.find(next_value) == relevant_stmts_2.end()) {
                continue;
            }
            table.add_row({next_key, next_value});
        }
    }

    return table;
}

auto NextEvaluator::eval_next(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_1);
    auto table = Table{{stmt_syn_1}};
    const auto stmt_candidates = read_facade->get_previous_of(stmt_num_2.value);
    for (const auto& candidate : stmt_candidates) {
        if (relevant_stmts.find(candidate) != relevant_stmts.end()) {
            table.add_row({candidate});
        }
    }

    return table;
}

auto NextEvaluator::eval_next(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_1);
    auto table = Table{{stmt_syn_1}};
    const auto all_next_keys = read_facade->get_all_next_keys();
    for (const auto& stmt : all_next_keys) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }

    return table;
}

auto NextEvaluator::eval_next(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_2);
    auto table = Table{{stmt_syn_2}};
    const auto stmt_candidates = read_facade->get_next_of(stmt_num_1.value);
    for (const auto& candidate : stmt_candidates) {
        if (relevant_stmts.find(candidate) != relevant_stmts.end()) {
            table.add_row({candidate});
        }
    }

    return table;
}

auto NextEvaluator::eval_next(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->has_next_relation(stmt_num_1.value, stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto NextEvaluator::eval_next(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    if (!read_facade->contains_next_key(stmt_num_1.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto NextEvaluator::eval_next(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_2);
    auto table = Table{{stmt_syn_2}};
    const auto all_next_values = read_facade->get_all_next_values();
    for (const auto& stmt : all_next_values) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }

    return table;
}

auto NextEvaluator::eval_next(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->contains_next_value(stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto NextEvaluator::eval_next(const WildCard&, const WildCard&) const -> OutputTable {
    if (!read_facade->has_next_relation()) {
        return Table{};
    }
    return UnitTable{};
}

} // namespace qps