#include "qps/evaluators/relationship/pattern_evaluator.hpp"

namespace qps{
auto PatternEvaluator::eval_pattern(const std::shared_ptr<ReadFacade> &read_facade, const std::shared_ptr<AssignSynonym>& assign_synonym) {
    return overloaded {
        // e.g. pattern a(x, _"v"_)
        [read_facade, assign_synonym](const std::shared_ptr<Synonym>& synonym, const qps::PartialMatch& partial_match) -> std::optional<Table> {
            auto table = Table{{assign_synonym, synonym}};
            const auto all_partial_matches = read_facade->get_all_assignments_rhs_partial(partial_match.expr.value);
            for (const auto& assign_stmt : all_partial_matches) {
                for (const auto& var : read_facade->get_vars_modified_by_statement(assign_stmt)) {
                    table.add_row({assign_stmt, var});
                }
            }
            return table;
        },

        // e.g. pattern a(x, _)
        [read_facade, assign_synonym](const std::shared_ptr<Synonym>& synonym, const qps::WildCard& wild_card) -> std::optional<Table> {
            auto table = Table{{assign_synonym, synonym}};
            for (const auto& assign_stmt : read_facade->get_assign_statements()) {
                for (const auto& var : read_facade->get_vars_modified_by_statement(assign_stmt)) {
                    table.add_row({assign_stmt, var});
                }
            }
            return table;
        },

        // e.g. pattern a("x", _"v"_)
        [read_facade, assign_synonym](const qps::QuotedIdent& quoted_ident, const qps::PartialMatch& partial_match) -> std::optional<Table> {
            auto table = Table{{assign_synonym}};
            const auto all_partial_matches = read_facade->get_all_assignments_lhs_rhs_partial(quoted_ident.get_value(), partial_match.expr.value);
            for (const auto& assign_stmt : all_partial_matches) {
                table.add_row({assign_stmt});
            }
            return table;
        },

        // e.g. pattern a("x", _)
        [read_facade, assign_synonym](const qps::QuotedIdent& quoted_ident, const qps::WildCard& wild_card) -> std::optional<Table> {
            auto table = Table{{assign_synonym}};
            const auto stmts_that_modify = read_facade->get_statements_that_modify_var(quoted_ident.get_value(), StatementType::Assign);
            for (const auto& stmt : stmts_that_modify) {
                table.add_row({stmt});
            }
            return table;
        },

        // e.g. pattern a(_, _"v"_)
        [read_facade, assign_synonym](const qps::WildCard& wild_card, const qps::PartialMatch& partial_match) -> std::optional<Table> {
            auto table = Table{{assign_synonym}};
            const auto all_partial_matches = read_facade->get_all_assignments_rhs_partial(partial_match.expr.value);
            for (const auto& stmt : all_partial_matches) {
                table.add_row({stmt});
            }
            return table;
        },

        // e.g. pattern a(_, _)
        [read_facade, assign_synonym](const qps::WildCard& wild_card_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            auto table = Table{{assign_synonym}};
            const auto all_assign_stmts = read_facade->get_assign_statements();
            for (const auto& stmt : all_assign_stmts) {
                table.add_row({stmt});
            }
            return table;
        },

        [assign_synonym](const auto &x, const auto& y) -> std::optional<Table> {
            return Table{{assign_synonym}};
        }
    };
}

auto PatternEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_pattern(read_facade, pattern.assign_synonym), pattern.ent_ref, pattern.expression_spec);
}

} // namespace qps