#include "qps/evaluators/relationship/follows_evaluator.hpp"

namespace qps {

auto FollowsEvaluator::eval_follows(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded{

        // e.g. Follows(s1, s2)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1, stmt_syn_2}};
            const auto follows_map = read_facade->get_all_follows();
            for (const auto& follows_pair : follows_map) {
                table.add_row({follows_pair.first, follows_pair.second});
            }
            return table;
        },

        // e.g. Follows(s1, 3)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const qps::Integer& stmt_num_2) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1}};
            // TODO: check w pkb team if this is the correct api call
            table.add_row({read_facade->get_follows_following(std::to_string(stmt_num_2.value))});
            return table;
        },

        // e.g. Follows(s1, _)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard&) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1}};
            const auto all_followed_stmts = read_facade->get_all_follows_keys();
            for (const auto& stmt : all_followed_stmts) {
                table.add_row({stmt});
            }
            return table;
        },

        // e.g. Follows(3, s2)
        [read_facade](const qps::Integer& stmt_num_1,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table{{stmt_syn_2}};
            // TODO: check w pkb team if this is the correct api call
            table.add_row({read_facade->get_follows_by(std::to_string(stmt_num_1.value))});
            return table;
        },

        // e.g. Follows(3, 4)
        [read_facade](const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            if (!read_facade->has_follows(std::to_string(stmt_num_1.value), std::to_string(stmt_num_2.value))) {
                return std::nullopt;
            }
            return Table{};
        },

        // e.g. Follows(3, _)
        [read_facade](const qps::Integer& stmt_num_1, const qps::WildCard&) -> std::optional<Table> {
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
        },

        // e.g. Follows(_, s2)
        [read_facade](const qps::WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table({stmt_syn_2});
            const auto all_following = read_facade->get_all_follows_values();
            for (const auto& stmt : all_following) {
                table.add_row({stmt});
            }
            return table;
        },

        // e.g. Follows(_, 3)
        [read_facade](const qps::WildCard&, const qps::Integer& stmt_num_2) -> std::optional<Table> {
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
        },

        // e.g. Follows(_, _)
        [read_facade](const qps::WildCard&, const qps::WildCard&) -> std::optional<Table> {
            if (read_facade->get_all_follows_keys().empty()) {
                return std::nullopt;
            }
            return Table{};
        }};
}

auto FollowsEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_follows(read_facade), follows.stmt1, follows.stmt2);
}
} // namespace qps