#include "qps/evaluators/clause_evaluators/relationship/follows_evaluator.hpp"

namespace qps {

auto FollowsEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_follows(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto FollowsEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), follows.stmt1, follows.stmt2);
}

auto FollowsEvaluator::eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    if (stmt_syn_1 == stmt_syn_2) {
        return Table{};
    }
    const auto relevant_stmts_1 = get_data(stmt_syn_1);
    const auto relevant_stmts_2 = get_data(stmt_syn_2);

    auto table = Table{{stmt_syn_1, stmt_syn_2}};
    const auto follows_map = read_facade->get_all_follows();
    for (const auto& follows_pair : follows_map) {
        if (relevant_stmts_1.find(follows_pair.first) == relevant_stmts_1.end()) {
            continue;
        }
        if (relevant_stmts_2.find(follows_pair.second) == relevant_stmts_1.end()) {
            continue;
        }
        table.add_row({follows_pair.first, follows_pair.second});
    }

    return table;
}

auto FollowsEvaluator::eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const qps::Integer& stmt_num_2) const -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_1);
    auto table = Table{{stmt_syn_1}};
    const auto candidate = read_facade->get_statement_followed_by(stmt_num_2.value);
    if (relevant_stmts.find(candidate) != relevant_stmts.end()) {
        table.add_row({candidate});
    }

    return table;
}

auto FollowsEvaluator::eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_1);
    auto table = Table{{stmt_syn_1}};
    const auto all_followed_stmts = read_facade->get_all_follows_keys();
    for (const auto& stmt : all_followed_stmts) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }

    return table;
}

auto FollowsEvaluator::eval_follows(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_2);
    auto table = Table{{stmt_syn_2}};
    const auto candidate = read_facade->get_statement_following(stmt_num_1.value);
    if (relevant_stmts.find(candidate) != relevant_stmts.end()) {
        table.add_row({candidate});
    }

    return table;
}

auto FollowsEvaluator::eval_follows(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->has_follows_relation(stmt_num_1.value, stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto FollowsEvaluator::eval_follows(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    if (!read_facade->contains_follows_key(stmt_num_1.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto FollowsEvaluator::eval_follows(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_syn_2);
    auto table = Table({stmt_syn_2});
    const auto all_following = read_facade->get_all_follows_values();
    for (const auto& stmt : all_following) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }
    return table;
}

auto FollowsEvaluator::eval_follows(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->contains_follows_value(stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto FollowsEvaluator::eval_follows(const WildCard&, const WildCard&) const -> OutputTable {
    if (read_facade->get_all_follows_keys().empty()) {
        return Table{};
    }
    return UnitTable{};
}

} // namespace qps