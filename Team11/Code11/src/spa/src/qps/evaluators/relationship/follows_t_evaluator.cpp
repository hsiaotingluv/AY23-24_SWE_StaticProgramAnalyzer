#include "qps/evaluators/relationship/follows_t_evaluator.hpp"

namespace qps {

auto FollowsTEvaluator::eval_follows_t(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded {

        // e.g. Follows*(s1, s2)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1, stmt_syn_2}};
            const auto follows_star_map= read_facade->get_all_follows_star();
            for (const auto& stmt_and_followers : follows_star_map) {
                for(const auto& follower : stmt_and_followers.second) {
                    table.add_row({stmt_and_followers.first, follower});
                }
            }
            return table;
        },

        // e.g. Follows*(s1, 3)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1}};
            // TODO: check w pkb team if this is the correct api call
            const auto followed_stmts = read_facade->get_follows_stars_following(std::to_string(stmt_num_2.value));
            for (const auto& stmt : followed_stmts){
                table.add_row({stmt});
            }
            return table;
        },

        // e.g. Follows*(s1, _)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1}};
            const auto all_followed_stmts = read_facade-> get_all_follows_star_keys();
            for (const auto& stmt : all_followed_stmts) {
                table.add_row({stmt});
            }
            return table;
        },

        // e.g. Follows*(3, s2)
        [read_facade](const qps::Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table({stmt_syn_2});
            // TODO: check w pkb team if this is the correct api call
            const auto all_followers_of_stmt = read_facade->get_follows_stars_by(std::to_string(stmt_num_1.value));
            for (const auto& follower : all_followers_of_stmt) {
                table.add_row({follower});
            }
            return table;
        },

        // e.g. Follows*(3, 4)
        [read_facade](const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            // TODO: improve pkv API, has_follows_star
            const auto follows_star_map= read_facade->get_all_follows_star();
            bool stmt_1_is_followed = false;
            bool stmt_2_is_following = false;
            const auto stmt_1_string = std::to_string(stmt_num_1.value);
            const auto stmt_2_string = std::to_string(stmt_num_2.value);
            for (const auto& stmt_and_followers : follows_star_map) {
                if (stmt_and_followers.first != stmt_1_string) {
                    continue;
                }
                stmt_1_is_followed = true;
                for(const auto& follower : stmt_and_followers.second) {
                    if (follower == stmt_2_string) {
                        stmt_2_is_following = true;
                        break;
                    }
                }
            }
            if (!stmt_1_is_followed || !stmt_2_is_following) {
                return std::nullopt;
            }
            return Table{};
        },

        // e.g. Follows*(3, _)
        [read_facade](const qps::Integer& stmt_num_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            // TODO: Improve pkb API: bool is_followed_by_something
            const auto all_followed_stmts = read_facade-> get_all_follows_star_keys();
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
        },

        // e.g. Follows*(_, s2)
        [read_facade](const qps::WildCard& wild_card_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table({stmt_syn_2});
            const auto all_following = read_facade-> get_all_follows_star_values();
            for (const auto& stmt : all_following) {
                table.add_row({stmt});
            }
            return table;
        },

        // e.g. Follows*(_, 3)
        [read_facade](const qps::WildCard& wild_card_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            // TODO: Improve pkb API: bool is_following_star_something
            const auto all_following_stmts = read_facade-> get_all_follows_star_values();
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
        },

        // e.g. Follows*(_, _)
        [read_facade](const qps::WildCard& wild_card_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            return Table{};
        }
    };
}

auto FollowsTEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_follows_t(read_facade), follows_t.stmt1, follows_t.stmt2);
}
}