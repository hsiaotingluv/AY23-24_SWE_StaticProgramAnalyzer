#include "qps/evaluators/pattern_clause_evaluator.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/expression_parser.hpp"
#include "qps/template_utils.hpp"
#include <optional>
#include <variant>

namespace qps {
auto pattern_clause_evaluator(const std::shared_ptr<ReadFacade>& read_facade,
                              const std::shared_ptr<PatternClause>& pattern_clause) -> std::optional<Table> {

    const auto& synonym = pattern_clause->assign_synonym;
    const auto& lhs = pattern_clause->ent_ref;
    const auto& rhs = pattern_clause->expression_spec;

    return std::visit(
        overloaded{
            [&read_facade, &synonym](const WildCard&, const WildCard&) -> std::optional<Table> {
                // If there is at least one assignment statement, return a table with one column
                const auto assignments = read_facade->get_assign_statements();
                if (assignments.empty()) {
                    return std::nullopt;
                }

                auto table = Table{{synonym}};
                for (const auto& assignment : assignments) {
                    table.add_row({{assignment}});
                }
                return table;
            },
            [&read_facade, &synonym](const std::shared_ptr<Synonym>& var_synonym,
                                     const WildCard&) -> std::optional<Table> {
                const auto assignments = read_facade->get_assign_statements();
                if (assignments.empty()) {
                    return std::nullopt;
                }

                auto table = Table{{synonym, var_synonym}};
                for (const auto& assign_stmt : assignments) {
                    const auto& variables = read_facade->get_vars_modified_by_statement(assign_stmt);
                    for (const auto& variable : variables) {
                        table.add_row({{assign_stmt, variable}});
                    }
                }
                return table;
            },
            [&read_facade, &synonym](const QuotedIdent& lhs, const WildCard&) -> std::optional<Table> {
                const auto& statements = read_facade->get_all_assignments_lhs(lhs.get_value());
                if (statements.empty()) {
                    return std::nullopt;
                }

                auto table = Table{{synonym}};
                for (const auto& statement : statements) {
                    table.add_row({{statement}});
                }
                return table;
            },
            [&read_facade, &synonym](const WildCard&, const PartialMatch& partial) -> std::optional<Table> {
                const auto& statements = read_facade->get_all_assignments_rhs_partial(partial.expr.value);
                if (statements.empty()) {
                    return std::nullopt;
                }
                auto table = Table{{synonym}};
                for (const auto& statement : statements) {
                    table.add_row({{statement}});
                }
                return table;
            },
            [&read_facade, &synonym](const std::shared_ptr<Synonym>& var_synonym,
                                     const PartialMatch& partial) -> std::optional<Table> {
                const auto& statements = read_facade->get_all_assignments_rhs_partial(partial.expr.value);
                if (statements.empty()) {
                    return std::nullopt;
                }

                auto table = Table{{synonym, var_synonym}};
                for (const auto& assign_stmt : statements) {
                    const auto& variables = read_facade->get_vars_modified_by_statement(assign_stmt);
                    for (const auto& variable : variables) {
                        table.add_row({{assign_stmt, variable}});
                    }
                }
                return table;
            },
            [&read_facade, &synonym](const QuotedIdent& lhs, const PartialMatch& partial) -> std::optional<Table> {
                const auto& statements =
                    read_facade->get_all_assignments_lhs_rhs_partial(lhs.get_value(), partial.expr.value);
                if (statements.empty()) {
                    return std::nullopt;
                }

                auto table = Table{{synonym}};
                for (const auto& statement : statements) {
                    table.add_row({{statement}});
                }
                return table;
            }},
        lhs, rhs);
}
} // namespace qps
