#include "qps/evaluators/relationship/modifies_s_evaluator.hpp"
#include "qps/evaluators/results_table.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <optional>

namespace qps {

auto ModifiesSEvaluator::eval_modifies(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded{
        [read_facade](const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                      const qps::WildCard&) -> std::optional<Table> {
            // TODO: Improve pkb API: Get all statement that modifies
            const auto relevant_stmts = stmt_synonym->scan(read_facade);
            auto table = Table{{stmt_synonym}};

            const auto variables = read_facade->get_variables();
            const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

            for (const auto& var : var_vec) {
                const auto statements = read_facade->get_statements_that_modify_var(var);
                const auto stmt_vec = std::vector<std::string>{statements.begin(), statements.end()};

                for (const auto& stmt : stmt_vec) {
                    if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
                        table.add_row({stmt});
                    }
                }
            }

            if (table.empty()) {
                return std::nullopt;
            }
            return table;
        },
        [read_facade](const qps::Integer& stmt_num, const qps::WildCard&) -> std::optional<Table> {
            auto does_statement_modify_var = false;
            const auto variables = read_facade->get_variables();
            const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

            for (const auto& var : var_vec) {
                if (read_facade->does_statement_modify_var(std::to_string(stmt_num.value), var)) {
                    does_statement_modify_var = true;
                    break;
                }
            }

            if (!does_statement_modify_var) {
                return std::nullopt;
            }
            return Table{};
        },
        [read_facade](const std::shared_ptr<qps::StmtSynonym>& synonym,
                      const std::shared_ptr<qps::VarSynonym>& var_syn) -> std::optional<Table> {
            // TODO: Improve pkb API: Get all statement that modifies and all variables that are modified
            const auto relevant_stmts = synonym->scan(read_facade);
            const auto relevant_variables = var_syn->scan(read_facade);

            const auto stmt_vec = std::vector<std::string>{relevant_stmts.begin(), relevant_stmts.end()};
            const auto var_vec = std::vector<std::string>{relevant_variables.begin(), relevant_variables.end()};

            auto table = Table{{synonym, var_syn}};

            for (const auto& stmt : stmt_vec) {
                for (const auto& v : var_vec) {
                    if (read_facade->does_statement_modify_var(stmt, v)) {
                        table.add_row({stmt, v});
                    }
                }
            }

            if (table.empty()) {
                return std::nullopt;
            }
            return table;
        },
        [read_facade](const qps::Integer& stmt_num,
                      const std::shared_ptr<qps::VarSynonym>& var) -> std::optional<Table> {
            const auto variables = read_facade->get_vars_modified_by_statement(std::to_string(stmt_num.value));
            auto table = Table{{var}};
            for (const auto& v : variables) {
                table.add_row({v});
            }

            if (table.empty()) {
                return std::nullopt;
            }
            return table;
        },
        [read_facade](const std::shared_ptr<qps::StmtSynonym>& synonym,
                      const qps::QuotedIdent& identifier) -> std::optional<Table> {
            const auto relevant_stmts = synonym->scan(read_facade);
            const auto relevant_stmt_vec = std::vector<std::string>{relevant_stmts.begin(), relevant_stmts.end()};

            const auto stmts = read_facade->get_statements_that_modify_var(identifier.get_value());
            auto table = Table{{synonym}};

            for (const auto& stmt : relevant_stmt_vec) {
                if (stmts.find(stmt) != stmts.end()) {
                    table.add_row({stmt});
                }
            }
            if (table.empty()) {
                return std::nullopt;
            }
            return table;
        },
        [read_facade](const qps::Integer& stmt_num, const qps::QuotedIdent& identifier) -> std::optional<Table> {
            const auto does_modify =
                read_facade->does_statement_modify_var(std::to_string(stmt_num.value), identifier.get_value());

            if (!does_modify) {
                return std::nullopt;
            }
            return Table{};
        }};
}

auto ModifiesSEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_modifies(read_facade), modifies_s.stmt, modifies_s.ent);
};

} // namespace qps