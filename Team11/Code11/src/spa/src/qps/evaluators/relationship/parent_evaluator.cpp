#include "qps/evaluators/relationship/parent_evaluator.hpp"

namespace qps {

auto ParentEvaluator::select_eval_method() const {
    return overloaded{
        // e.g. Parent(s1, s2)
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
               const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> OutputTable {
            return eval_parent(stmt_syn_1, stmt_syn_2);
        },
        // e.g. Parent(s1, 3)
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2) -> OutputTable {
            return eval_parent(stmt_syn_1, stmt_num_2);
        },
        // e.g. Parent(s1, _)
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard& wild_card_2) -> OutputTable {
            return eval_parent(stmt_syn_1, wild_card_2);
        },
        // e.g. Parent(3, s2)
        [this](const qps::Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> OutputTable {
            return eval_parent(stmt_num_1, stmt_syn_2);
        },
        // e.g. Parent(3, 4)
        [this](const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) -> OutputTable {
            return eval_parent(stmt_num_1, stmt_num_2);
        },
        // e.g. Parent(3, _)
        [this](const qps::Integer& stmt_num_1, const qps::WildCard& wild_card_2) -> OutputTable {
            return eval_parent(stmt_num_1, wild_card_2);
        },
        // e.g. Parent(_, s2)
        [this](const qps::WildCard& wild_card_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> OutputTable {
            return eval_parent(wild_card_1, stmt_syn_2);
        },
        // e.g. Parent(_, 3)
        [this](const qps::WildCard& wild_card_1, const qps::Integer& stmt_num_2) -> OutputTable {
            return eval_parent(wild_card_1, stmt_num_2);
        },
        // e.g. Parent(_, _)
        [this](const qps::WildCard& wild_card_1, const qps::WildCard& wild_card_2) -> OutputTable {
            return eval_parent(wild_card_1, wild_card_2);
        }};
}

auto ParentEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), parent.stmt1, parent.stmt2);
}

auto ParentEvaluator::eval_parent(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                  const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    if (stmt_syn_1 == stmt_syn_2) {
        return Table{};
    }
    const auto relevant_stmts_1 = stmt_syn_1->scan(read_facade);
    const auto relevant_stmts_2 = stmt_syn_2->scan(read_facade);

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
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto parent_candidate = read_facade->get_parent_of(stmt_num_2.value);
    if (relevant_stmts.find(parent_candidate) != relevant_stmts.end()) {
        table.add_row({parent_candidate});
    }

    return table;
}

auto ParentEvaluator::eval_parent(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
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
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
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
    // TODO: Improve pkb API: bool is_a_parent
    const auto all_parents = read_facade->get_all_parent_keys();
    bool is_parent = false;
    const auto stmt_num = stmt_num_1.value;
    for (const auto& parent_name : all_parents) {
        if (stmt_num == parent_name) {
            is_parent = true;
            break;
        }
    }
    if (!is_parent) {
        return Table{};
    }
    return UnitTable{};
}

auto ParentEvaluator::eval_parent(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
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
    // TODO: Improve pkb API: bool has_a_parent
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

auto ParentEvaluator::eval_parent(const WildCard&, const WildCard&) const -> OutputTable {
    if (read_facade->get_all_parent_keys().empty()) {
        return Table{};
    }
    return UnitTable{};
}
} // namespace qps