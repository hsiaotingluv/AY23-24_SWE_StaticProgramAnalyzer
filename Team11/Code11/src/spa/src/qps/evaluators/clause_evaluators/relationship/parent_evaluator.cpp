#include "qps/evaluators/clause_evaluators/relationship/parent_evaluator.hpp"

namespace qps {

auto ParentEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_parent(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto ParentEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), parent.stmt1, parent.stmt2);
}

auto ParentEvaluator::eval_parent(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                  const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    if (stmt_syn_1 == stmt_syn_2) {
        return Table{};
    }
    const auto relevant_stmts_1 = get_data(stmt_syn_1);
    const auto relevant_stmts_2 = get_data(stmt_syn_2);

    auto table = Table{{stmt_syn_1, stmt_syn_2}};
    // TODO: Improve pkb API: Get all parent-child pairs
    const auto parents_map = read_facade->get_all_parent();
    for (const auto& parent_child_set : parents_map) {
        if (relevant_stmts_1.find(parent_child_set.first) == relevant_stmts_1.end()) {
            continue;
        }

        for (const auto& child : parent_child_set.second) {
            if (relevant_stmts_2.find(child) == relevant_stmts_2.end()) {
                continue;
            }
            table.add_row({parent_child_set.first, child});
        }
    }

    return table;
}

auto ParentEvaluator::eval_parent(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_1);
    auto table = Table{{stmt_syn_1}};
    const auto parent_candidate = read_facade->get_parent_of(stmt_num_2.value);
    if (relevant_stmts.find(parent_candidate) != relevant_stmts.end()) {
        table.add_row({parent_candidate});
    }

    return table;
}

auto ParentEvaluator::eval_parent(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_1);
    auto table = Table{{stmt_syn_1}};
    const auto all_parents = read_facade->get_all_parent_keys();
    for (const auto& parent_name : all_parents) {
        if (relevant_stmts.find(parent_name) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({parent_name});
    }

    return table;
}

auto ParentEvaluator::eval_parent(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_2);
    auto table = Table({stmt_syn_2});
    const auto all_children_of_stmt = read_facade->get_children_of(stmt_num_1.value);
    for (const auto& child_name : all_children_of_stmt) {
        if (relevant_stmts.find(child_name) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({child_name});
    }

    return table;
}

auto ParentEvaluator::eval_parent(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->has_parent_relation(stmt_num_1.value, stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto ParentEvaluator::eval_parent(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    if (!read_facade->contains_parent_key(stmt_num_1.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto ParentEvaluator::eval_parent(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_2);
    auto table = Table({stmt_syn_2});
    const auto all_children = read_facade->get_all_parent_values();
    for (const auto& child_name : all_children) {
        if (relevant_stmts.find(child_name) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({child_name});
    }

    return table;
}

auto ParentEvaluator::eval_parent(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->contains_parent_value(stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto ParentEvaluator::eval_parent(const WildCard&, const WildCard&) const -> OutputTable {
    if (read_facade->get_all_parent_keys().empty()) {
        return Table{};
    }
    return UnitTable{};
}
} // namespace qps