#include "qps/evaluators/relationship/parent_t_evaluator.hpp"
#include "qps/evaluators/results_table.hpp"

namespace qps {

auto ParentTEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_parent_t(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto ParentTEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), parent_t.stmt1, parent_t.stmt2);
}

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                     const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    if (stmt_syn_1 == stmt_syn_2) {
        return Table{};
    }
    const auto relevant_stmts_1 = stmt_syn_1->scan(read_facade);
    const auto relevant_stmts_2 = stmt_syn_2->scan(read_facade);

    auto table = Table{{stmt_syn_1, stmt_syn_2}};
    // TODO: Improve pkb API: Get all parent-star-child pairs
    const auto parents_map = read_facade->get_all_parent_star();
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

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto ancestors = read_facade->get_parent_star_of(stmt_num_2.value);
    for (const auto& ancestor : ancestors) {
        if (relevant_stmts.find(ancestor) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({ancestor});
    }
    return table;
}

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto all_parents = read_facade->get_all_parent_star_keys();
    for (const auto& parent_name : all_parents) {
        if (relevant_stmts.find(parent_name) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({parent_name});
    }
    return table;
}

auto ParentTEvaluator::eval_parent_t(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table({stmt_syn_2});
    const auto all_descendants_of_stmt = read_facade->get_children_star_of(stmt_num_1.value);
    for (const auto& descendant : all_descendants_of_stmt) {
        if (relevant_stmts.find(descendant) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({descendant});
    }
    return table;
}

auto ParentTEvaluator::eval_parent_t(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->has_parent_star_relation(stmt_num_1.value, stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto ParentTEvaluator::eval_parent_t(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    // TODO: Improve pkb API: bool is_a_parent_star
    const auto all_ancestors = read_facade->get_all_parent_star_keys();
    bool is_ancestor = false;
    const auto stmt_num = stmt_num_1.value;
    for (const auto& ancestor_name : all_ancestors) {
        if (stmt_num == ancestor_name) {
            is_ancestor = true;
            break;
        }
    }
    if (!is_ancestor) {
        return Table{};
    }
    return UnitTable{};
}

auto ParentTEvaluator::eval_parent_t(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table({stmt_syn_2});
    const auto all_descendants = read_facade->get_all_parent_star_values();
    for (const auto& descendant_name : all_descendants) {
        if (relevant_stmts.find(descendant_name) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({descendant_name});
    }

    return table;
}

auto ParentTEvaluator::eval_parent_t(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    // TODO: Improve pkb API: bool has_a_parent_star
    const auto all_children = read_facade->get_all_parent_values();
    bool has_a_parent = false;
    const auto stmt_num = stmt_num_2.value;
    for (const auto& child_name : all_children) {
        if (stmt_num == child_name) {
            has_a_parent = true;
            break;
        }
    }
    if (!has_a_parent) {
        return Table{};
    }
    return UnitTable{};
}

auto ParentTEvaluator::eval_parent_t(const WildCard&, const WildCard&) const -> OutputTable {
    if (read_facade->get_all_parent_star_keys().empty()) {
        return Table{};
    }
    return UnitTable{};
}
} // namespace qps