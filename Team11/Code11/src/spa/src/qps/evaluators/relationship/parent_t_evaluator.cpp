#include "qps/evaluators/relationship/parent_t_evaluator.hpp"

namespace qps {

[[nodiscard]] auto ParentTEvaluator::select_eval_method() const {
    return overloaded{
        // e.g. Parent*(s1, s2)
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
               const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            return eval_parent_t(stmt_syn_1, stmt_syn_2);
        },
        // e.g. Parent*(s1, 3)
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            return eval_parent_t(stmt_syn_1, stmt_num_2);
        },
        // e.g. Parent*(s1, _)
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
               const qps::WildCard& wild_card_2) -> std::optional<Table> {
            return eval_parent_t(stmt_syn_1, wild_card_2);
        },
        // e.g. Parent*(3, s2)
        [this](const qps::Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            return eval_parent_t(stmt_num_1, stmt_syn_2);
        },
        // e.g. Parent*(3, 4)
        [this](const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            return eval_parent_t(stmt_num_1, stmt_num_2);
        },
        // e.g. Parent*(3, _)
        [this](const qps::Integer& stmt_num_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            return eval_parent_t(stmt_num_1, wild_card_2);
        },
        // e.g. Parent*(_, s2)
        [this](const qps::WildCard& wild_card_1,
               const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            return eval_parent_t(wild_card_1, stmt_syn_2);
        },
        // e.g. Parent*(_, 3)
        [this](const qps::WildCard& wild_card_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            return eval_parent_t(wild_card_1, stmt_num_2);
        },
        // e.g. Parent*(_, _)
        [this](const qps::WildCard& wild_card_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            return eval_parent_t(wild_card_1, wild_card_2);
        }};
}

auto ParentTEvaluator::evaluate() const -> std::optional<Table> {
    return std::visit(select_eval_method(), parent_t.stmt1, parent_t.stmt2);
}

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                     const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> std::optional<Table> {
    if (stmt_syn_1 == stmt_syn_2) {
        return std::nullopt;
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

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> std::optional<Table> {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto ancestors = read_facade->get_parent_star_of(stmt_num_2.value);
    for (const auto& ancestor : ancestors) {
        if (relevant_stmts.find(ancestor) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({ancestor});
    }
    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const
    -> std::optional<Table> {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto all_parents = read_facade->get_all_parent_star_keys();
    for (const auto& parent_name : all_parents) {
        if (relevant_stmts.find(parent_name) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({parent_name});
    }
    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ParentTEvaluator::eval_parent_t(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> std::optional<Table> {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table({stmt_syn_2});
    const auto all_descendants_of_stmt = read_facade->get_children_star_of(stmt_num_1.value);
    for (const auto& descendant : all_descendants_of_stmt) {
        if (relevant_stmts.find(descendant) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({descendant});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ParentTEvaluator::eval_parent_t(const Integer& stmt_num_1, const Integer& stmt_num_2) const
    -> std::optional<Table> {
    if (!read_facade->has_parent_star_relation(stmt_num_1.value, stmt_num_2.value)) {
        return std::nullopt;
    }
    return Table{};
}

auto ParentTEvaluator::eval_parent_t(const Integer& stmt_num_1, const WildCard&) const -> std::optional<Table> {
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
        return std::nullopt;
    }
    return Table{};
}

auto ParentTEvaluator::eval_parent_t(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> std::optional<Table> {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table({stmt_syn_2});
    const auto all_descendants = read_facade->get_all_parent_star_values();
    for (const auto& descendant_name : all_descendants) {
        if (relevant_stmts.find(descendant_name) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({descendant_name});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ParentTEvaluator::eval_parent_t(const WildCard&, const Integer& stmt_num_2) const -> std::optional<Table> {
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
        return std::nullopt;
    }
    return Table{};
}

auto ParentTEvaluator::eval_parent_t(const WildCard&, const WildCard&) const -> std::optional<Table> {
    if (read_facade->get_all_parent_star_keys().empty()) {
        return std::nullopt;
    }
    return Table{};
}
} // namespace qps