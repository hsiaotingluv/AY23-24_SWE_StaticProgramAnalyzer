#include "qps/evaluators/relationship/follows_evaluator.hpp"

namespace qps {

auto FollowsEvaluator::select_eval_method() const {
    return overloaded{
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
               const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> OutputTable {
            return eval_follows(stmt_syn_1, stmt_syn_2);
        },
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2) -> OutputTable {
            return eval_follows(stmt_syn_1, stmt_num_2);
        },
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard& wild_card_2) -> OutputTable {
            return eval_follows(stmt_syn_1, wild_card_2);
        },
        [this](const qps::Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> OutputTable {
            return eval_follows(stmt_num_1, stmt_syn_2);
        },
        [this](const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) -> OutputTable {
            return eval_follows(stmt_num_1, stmt_num_2);
        },
        [this](const qps::Integer& stmt_num_1, const qps::WildCard& wild_card_2) -> OutputTable {
            return eval_follows(stmt_num_1, wild_card_2);
        },
        [this](const qps::WildCard& wild_card_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> OutputTable {
            return eval_follows(wild_card_1, stmt_syn_2);
        },
        [this](const qps::WildCard& wild_card_1, const qps::Integer& stmt_num_2) -> OutputTable {
            return eval_follows(wild_card_1, stmt_num_2);
        },

        [this](const qps::WildCard& wild_card_1, const qps::WildCard& wild_card_2) -> OutputTable {
            return eval_follows(wild_card_1, wild_card_2);
        }};
}

auto FollowsEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), follows.stmt1, follows.stmt2);
}

auto FollowsEvaluator::eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    if (stmt_syn_1 == stmt_syn_2) {
        return Table{};
    }
    const auto relevant_stmts_1 = stmt_syn_1->scan(read_facade);
    const auto relevant_stmts_2 = stmt_syn_2->scan(read_facade);

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
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto candidate = read_facade->get_statement_followed_by(stmt_num_2.value);
    if (relevant_stmts.find(candidate) != relevant_stmts.end()) {
        table.add_row({candidate});
    }

    return table;
}

auto FollowsEvaluator::eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
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
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
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
    // TODO: Improve pkb API: bool is_followed_by_something
    const auto all_followed_stmts = read_facade->get_all_follows_keys();
    bool is_followed = false;
    const auto stmt_num = stmt_num_1.value;
    for (const auto& stmt : all_followed_stmts) {
        if (stmt_num == stmt) {
            is_followed = true;
            break;
        }
    }
    if (!is_followed) {
        return Table{};
    }
    return UnitTable{};
}

auto FollowsEvaluator::eval_follows(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
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
    // TODO: Improve pkb API: bool is_following_something
    const auto all_following_stmts = read_facade->get_all_follows_values();
    bool is_following = false;
    const auto stmt_num = stmt_num_2.value;
    for (const auto& stmt : all_following_stmts) {
        if (stmt_num == stmt) {
            is_following = true;
            break;
        }
    }
    if (!is_following) {
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