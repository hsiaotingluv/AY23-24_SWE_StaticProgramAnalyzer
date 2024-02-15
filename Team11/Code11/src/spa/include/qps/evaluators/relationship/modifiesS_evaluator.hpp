#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/entities/entity_scanner.hpp"
#include "qps/evaluators/results_map.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <optional>
#include <variant>

namespace qps {
const auto to_synonym = overloaded{[](const auto& x) {
    return Synonym{x};
}};

const auto ModifiesS_evaluator = [](std::shared_ptr<ReadFacade> read_facade) {
    return overloaded{
        [read_facade](const qps::StmtSynonym& stmt_synonym, const qps::WildCard& wildcard2) -> std::optional<Table> {
            // TODO: Improve pkb API: Get all statement that modifies
            const auto relevant_stmts = std::visit(scan_entities(read_facade), stmt_synonym);
            const auto synonym = std::visit(to_synonym, stmt_synonym);
            auto table = Table{{synonym}};

            const auto variables = read_facade->get_variables();
            const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

            for (const auto& var : var_vec) {
                const auto statements = read_facade->get_statements_that_modify_var(var);
                const auto stmt_vec = std::vector<std::string>{statements.begin(), statements.end()};

                for (const auto& stmt : stmt_vec) {
                    if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
                        table.add_row({static_cast<unsigned int>(std::stoi(stmt))});
                    }
                }
            }
            return table;
        },
        [read_facade](const qps::Integer& stmt_num, const qps::WildCard& wildcard2) -> std::optional<Table> {
            // TODO: Improve pkb API: Does statement modify var?
            auto does_statement_modify_var = false;
            const auto variables = read_facade->get_variables();
            const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

            for (const auto& var : var_vec) {
                const auto statements = read_facade->get_statements_that_modify_var(var);
                if (statements.find(std::to_string(stmt_num.value)) != statements.end()) {
                    does_statement_modify_var = true;
                    break;
                }
            }

            if (!does_statement_modify_var) {
                return std::nullopt;
            }
            return Table{};
        },
        [read_facade](const qps::StmtSynonym& synonym, const qps::VarSynonym& var) {
            // TODO: Improve pkb API: Get all statement that modifies and all variables that are modified
            const auto statements = read_facade->get_all_statements();
            const auto variables = read_facade->get_variables();
            const auto stmt_vec = std::vector<std::string>{statements.begin(), statements.end()};
            const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

            auto table = Table{{std::visit(to_synonym, synonym), var}};

            for (const auto& stmt : stmt_vec) {
                for (const auto& v : var_vec) {
                    if (read_facade->does_statement_modify_var(stmt, v)) {
                        results_map.update_mapping(synonym, stmt);
                        results_map.update_mapping(var, v);
                    }
                }
            }
            return;
        },
        [read_facade, &results_map](const qps::Integer& stmt_num, const qps::VarSynonym& var) {
            const auto variables = read_facade->get_vars_modified_by_statement(std::to_string(stmt_num.value));
            results_map.update_mapping(var, variables);
            return;
        },
        [read_facade, &results_map](const qps::StmtSynonym& synonym, const qps::QuotedIdent& identifier) {
            const auto stmts = read_facade->get_statements_that_modify_var(identifier.get_value());

            results_map.update_mapping(synonym, stmts);

            return;
        },
        [read_facade, &results_map](const qps::Integer& stmt_num, const qps::QuotedIdent& identifier) {
            const auto does_modify =
                read_facade->does_statement_modify_var(std::to_string(stmt_num.value), identifier.get_value());

            if (!does_modify) {
                results_map.set_unsatisfiable();
            } else {
                // TODO: this no longer holds if we allow chaining of clauses
                results_map.set_valid();
            }
            return;
        },
        [](const auto& x) {
            return std::unordered_set<std::string>{};
        }};
};
} // namespace qps
