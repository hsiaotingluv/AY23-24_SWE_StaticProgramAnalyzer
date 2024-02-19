#include "qps/evaluators/relationship/parent_evaluator.hpp"

namespace qps {

auto ParentEvaluator::eval_parent(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded{

        // e.g. Parent(s1, s2)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1, stmt_syn_2}};
            // TODO: Improve pkb API: Get all parent-child pairs
            const auto parents_map = read_facade->get_all_parent();
            for (const auto& parent_child_set : parents_map) {
                for (const auto& child : parent_child_set.second) {
                    table.add_row({parent_child_set.first, child});
                }
            }
            return table;
        },

        // e.g. Parent(s1, 3)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const qps::Integer& stmt_num_2) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1}};
            table.add_row({read_facade->get_parent(std::to_string(stmt_num_2.value))});
            return table;
        },

        // e.g. Parent(s1, _)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard&) -> std::optional<Table> {
            auto table = Table{{stmt_syn_1}};
            const auto all_parents = read_facade->get_all_parent_keys();
            for (const auto& parent_name : all_parents) {
                table.add_row({parent_name});
            }
            return table;
        },

        // e.g. Parent(3, s2)
        [read_facade](const qps::Integer& stmt_num_1,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table({stmt_syn_2});
            const auto all_children_of_stmt = read_facade->get_parent_children(std::to_string(stmt_num_1.value));
            for (const auto& child_name : all_children_of_stmt) {
                table.add_row({child_name});
            }
            return table;
        },

        // e.g. Parent(3, 4)
        [read_facade](const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            if (!read_facade->has_parent(std::to_string(stmt_num_1.value), std::to_string(stmt_num_2.value))) {
                return std::nullopt;
            }
            return Table{};
        },

        // e.g. Parent(3, _)
        [read_facade](const qps::Integer& stmt_num_1, const qps::WildCard&) -> std::optional<Table> {
            // TODO: Improve pkb API: bool is_a_parent
            const auto all_parents = read_facade->get_all_parent_keys();
            bool is_parent = false;
            const auto stmt_num = std::to_string(stmt_num_1.value);
            for (const auto& parent_name : all_parents) {
                if (stmt_num == parent_name) {
                    is_parent = true;
                    break;
                }
            }
            if (!is_parent) {
                return std::nullopt;
            }
            return Table{};
        },

        // e.g. Parent(_, s2)
        [read_facade](const qps::WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            auto table = Table({stmt_syn_2});
            const auto all_children = read_facade->get_all_parent_values();
            for (const auto& child_name : all_children) {
                table.add_row({child_name});
            }
            return table;
        },

        // e.g. Parent(_, 3)
        [read_facade](const qps::WildCard&, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            // TODO: Improve pkb API: bool has_a_parent
            const auto all_children = read_facade->get_all_parent_values();
            bool has_a_parent = false;
            const auto stmt_num = std::to_string(stmt_num_2.value);
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
        },

        // e.g. Parent(_, _)
        [read_facade](const qps::WildCard&, const qps::WildCard&) -> std::optional<Table> {
            if (read_facade->get_all_parent_keys().empty()) {
                return std::nullopt;
            }
            return Table{};
        }};
}

auto ParentEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_parent(read_facade), parent.stmt1, parent.stmt2);
}
} // namespace qps