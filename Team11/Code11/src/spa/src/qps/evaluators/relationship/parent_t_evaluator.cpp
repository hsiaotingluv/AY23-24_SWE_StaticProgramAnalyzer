#include "qps/evaluators/relationship/parent_t_evaluator.hpp"
#include "qps/evaluators/entities/entity_scanner.hpp"

namespace qps {

auto ParentTEvaluator::eval_parent_t(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded{

        // e.g. Parent*(s1, s2)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            if (stmt_syn_1 == stmt_syn_2) {
                return std::nullopt;
            }
            const auto relevant_stmts_1 = scan_entities(read_facade, stmt_syn_1);
            const auto relevant_stmts_2 = scan_entities(read_facade, stmt_syn_2);

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
        },

        // e.g. Parent*(s1, 3)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const qps::Integer& stmt_num_2) -> std::optional<Table> {
            const auto relevant_stmts = scan_entities(read_facade, stmt_syn_1);
            auto table = Table{{stmt_syn_1}};
            const auto ancestors = read_facade->get_star_parent(std::to_string(stmt_num_2.value));
            for (const auto& ancestor : ancestors) {
                if (relevant_stmts.find(ancestor) == relevant_stmts.end()) {
                    continue;
                }
                table.add_row({ancestor});
            }
            return table;
        },

        // e.g. Parent*(s1, _)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard&) -> std::optional<Table> {
            const auto relevant_stmts = scan_entities(read_facade, stmt_syn_1);
            auto table = Table{{stmt_syn_1}};
            const auto all_parents = read_facade->get_all_parent_star_keys();
            for (const auto& parent_name : all_parents) {
                if (relevant_stmts.find(parent_name) == relevant_stmts.end()) {
                    continue;
                }
                table.add_row({parent_name});
            }
            return table;
        },

        // e.g. Parent*(3, s2)
        [read_facade](const qps::Integer& stmt_num_1,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            const auto relevant_stmts = scan_entities(read_facade, stmt_syn_2);
            auto table = Table({stmt_syn_2});
            const auto all_descendants_of_stmt =
                read_facade->get_parent_star_children(std::to_string(stmt_num_1.value));
            for (const auto& descendant : all_descendants_of_stmt) {
                if (relevant_stmts.find(descendant) == relevant_stmts.end()) {
                    continue;
                }
                table.add_row({descendant});
            }
            return table;
        },

        // e.g. Parent*(3, 4)
        [read_facade](const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            if (!read_facade->has_parent_star(std::to_string(stmt_num_1.value), std::to_string(stmt_num_2.value))) {
                return std::nullopt;
            }
            return Table{};
        },

        // e.g. Parent*(3, _)
        [read_facade](const qps::Integer& stmt_num_1, const qps::WildCard&) -> std::optional<Table> {
            // TODO: Improve pkb API: bool is_a_parent_star
            const auto all_ancestors = read_facade->get_all_parent_star_keys();
            bool is_ancestor = false;
            const auto stmt_num = std::to_string(stmt_num_1.value);
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
        },

        // e.g. Parent*(_, s2)
        [read_facade](const qps::WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table> {
            const auto relevant_stmts = scan_entities(read_facade, stmt_syn_2);
            auto table = Table({stmt_syn_2});
            const auto all_descendants = read_facade->get_all_parent_star_values();
            for (const auto& descendant_name : all_descendants) {
                if (relevant_stmts.find(descendant_name) == relevant_stmts.end()) {
                    continue;
                }
                table.add_row({descendant_name});
            }
            return table;
        },

        // e.g. Parent*(_, 3)
        [read_facade](const qps::WildCard&, const qps::Integer& stmt_num_2) -> std::optional<Table> {
            // TODO: Improve pkb API: bool has_a_parent_star
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

        // e.g. Parent*(_, _)
        [read_facade](const qps::WildCard&, const qps::WildCard&) -> std::optional<Table> {
            return Table{};
        }};
}

auto ParentTEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_parent_t(read_facade), parent_t.stmt1, parent_t.stmt2);
}
} // namespace qps