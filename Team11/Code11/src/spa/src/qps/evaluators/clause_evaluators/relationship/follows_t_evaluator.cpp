#include "qps/evaluators/clause_evaluators/relationship/follows_t_evaluator.hpp"

namespace qps {

auto FollowsTEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_follows_t(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto FollowsTEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), follows_t.stmt1, follows_t.stmt2);
}

auto FollowsTEvaluator::eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                       const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    if (stmt_syn_1 == stmt_syn_2) {
        return Table{};
    }
    const auto relevant_stmts_1 = stmt_syn_1->scan(read_facade);
    const auto relevant_stmts_2 = stmt_syn_2->scan(read_facade);

    auto table = Table{{stmt_syn_1, stmt_syn_2}};
    const auto follows_star_map = read_facade->get_all_follows_star();
    for (const auto& stmt_and_followers : follows_star_map) {
        if (relevant_stmts_1.find(stmt_and_followers.first) == relevant_stmts_1.end()) {
            continue;
        }

        for (const auto& follower : stmt_and_followers.second) {
            if (relevant_stmts_2.find(follower) == relevant_stmts_1.end()) {
                continue;
            }
            table.add_row({stmt_and_followers.first, follower});
        }
    }

    return table;
}

auto FollowsTEvaluator::eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto followed_stmts = read_facade->get_follows_stars_by(stmt_num_2.value);
    for (const auto& stmt : followed_stmts) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }

    return table;
}

auto FollowsTEvaluator::eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto all_followed_stmts = read_facade->get_all_follows_star_keys();
    for (const auto& stmt : all_followed_stmts) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }

    return table;
}

auto FollowsTEvaluator::eval_follows_t(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table({stmt_syn_2});
    const auto all_followers_of_stmt = read_facade->get_follows_stars_following(stmt_num_1.value);
    for (const auto& follower : all_followers_of_stmt) {
        if (relevant_stmts.find(follower) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({follower});
    }

    return table;
}

auto FollowsTEvaluator::eval_follows_t(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->has_follows_star_relation(stmt_num_1.value, stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto FollowsTEvaluator::eval_follows_t(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    if (!read_facade->contains_follows_star_key(stmt_num_1.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto FollowsTEvaluator::eval_follows_t(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table({stmt_syn_2});
    const auto all_following = read_facade->get_all_follows_star_values();
    for (const auto& stmt : all_following) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }

    return table;
}

auto FollowsTEvaluator::eval_follows_t(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    if (!read_facade->contains_follows_star_value(stmt_num_2.value)) {
        return Table{};
    }
    return UnitTable{};
}

auto FollowsTEvaluator::eval_follows_t(const WildCard&, const WildCard&) const -> OutputTable {
    if (read_facade->get_all_follows_keys().empty()) {
        return Table{};
    }
    return UnitTable{};
}
} // namespace qps