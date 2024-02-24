#include "qps/evaluators/relationship/follows_evaluator.hpp"

namespace qps {

auto FollowsEvaluator::select_eval_method() {
    return overloaded{
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
               const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            return eval_follows(stmt_syn_1, stmt_syn_2);
        },
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            return eval_follows(stmt_syn_1, stmt_num_2);
        },
        [this](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
               const qps::WildCard& wild_card_2) -> std::optional<Table> {
            return eval_follows(stmt_syn_1, wild_card_2);
        },
        [this](const qps::Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            return eval_follows(stmt_num_1, stmt_syn_2);
        },
        [this](const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            return eval_follows(stmt_num_1, stmt_num_2);
        },
        [this](const qps::Integer& stmt_num_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            return eval_follows(stmt_num_1, wild_card_2);
        },
        [this](const qps::WildCard& wild_card_1,
               const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            return eval_follows(wild_card_1, stmt_syn_2);
        },
        [this](const qps::WildCard& wild_card_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            return eval_follows(wild_card_1, stmt_num_2);
        },

        [this](const qps::WildCard& wild_card_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            return eval_follows(wild_card_1, wild_card_2);
        }};
}

auto FollowsEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(select_eval_method(), follows.stmt1, follows.stmt2);
}

auto FollowsEvaluator::eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
    if (stmt_syn_1 == stmt_syn_2) {
        return std::nullopt;
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

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto FollowsEvaluator::eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2)
    -> std::optional<Table> {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto candidate = read_facade->get_follows_by(std::to_string(stmt_num_2.value));
    if (relevant_stmts.find(candidate) != relevant_stmts.end()) {
        table.add_row({candidate});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto FollowsEvaluator::eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&)
    -> std::optional<Table> {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto all_followed_stmts = read_facade->get_all_follows_keys();
    for (const auto& stmt : all_followed_stmts) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto FollowsEvaluator::eval_follows(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2)
    -> std::optional<Table> {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table{{stmt_syn_2}};
    const auto candidate = read_facade->get_follows_following(std::to_string(stmt_num_1.value));
    if (relevant_stmts.find(candidate) != relevant_stmts.end()) {
        table.add_row({candidate});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto FollowsEvaluator::eval_follows(const Integer& stmt_num_1, const Integer& stmt_num_2) -> std::optional<Table> {
    if (!read_facade->has_follows(std::to_string(stmt_num_1.value), std::to_string(stmt_num_2.value))) {
        return std::nullopt;
    }
    return Table{};
}

auto FollowsEvaluator::eval_follows(const Integer& stmt_num_1, const WildCard&) -> std::optional<Table> {
    // TODO: Improve pkb API: bool is_followed_by_something
    const auto all_followed_stmts = read_facade->get_all_follows_keys();
    bool is_followed = false;
    const auto stmt_num = std::to_string(stmt_num_1.value);
    for (const auto& stmt : all_followed_stmts) {
        if (stmt_num == stmt) {
            is_followed = true;
            break;
        }
    }
    if (!is_followed) {
        return std::nullopt;
    }
    return Table{};
}

auto FollowsEvaluator::eval_follows(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2)
    -> std::optional<Table> {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table({stmt_syn_2});
    const auto all_following = read_facade->get_all_follows_values();
    for (const auto& stmt : all_following) {
        if (relevant_stmts.find(stmt) == relevant_stmts.end()) {
            continue;
        }
        table.add_row({stmt});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto FollowsEvaluator::eval_follows(const WildCard&, const Integer& stmt_num_2) -> std::optional<Table> {
    // TODO: Improve pkb API: bool is_following_something
    const auto all_following_stmts = read_facade->get_all_follows_values();
    bool is_following = false;
    const auto stmt_num = std::to_string(stmt_num_2.value);
    for (const auto& stmt : all_following_stmts) {
        if (stmt_num == stmt) {
            is_following = true;
            break;
        }
    }
    if (!is_following) {
        return std::nullopt;
    }
    return Table{};
}

auto FollowsEvaluator::eval_follows(const WildCard&, const WildCard&) -> std::optional<Table> {
    if (read_facade->get_all_follows_keys().empty()) {
        return std::nullopt;
    }
    return Table{};
}

} // namespace qps