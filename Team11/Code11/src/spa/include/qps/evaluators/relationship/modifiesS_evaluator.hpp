#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/results_map.hpp"
#include "qps/evaluators/type_traits.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <memory>

namespace qps {
const auto ModifiesS_evaluator = [](std::shared_ptr<ReadFacade> read_facade, ResultsMap& results_map) {
    return overloaded{[read_facade, &results_map](const qps::StmtSynonym& synonym, const qps::WildCard&) {
                          // TODO: Improve pkb API: Get all statement that modifies
                          const auto variables = read_facade->get_variables();
                          const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

                          for (const auto& var : var_vec) {
                              const auto statements = read_facade->get_statements_that_modify_var(var);
                              const auto stmt_vec = std::vector<std::string>{statements.begin(), statements.end()};

                              for (const auto& stmt : stmt_vec) {
                                  results_map.update_mapping(synonym, stmt);
                              }
                          }
                          return;
                      },
                      [read_facade, &results_map](const qps::Integer& stmt_num, const qps::WildCard&) {
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
                              results_map.set_unsatisfiable();
                          } else {
                              // TODO: this no longer holds if we allow chaining of clauses
                              results_map.set_valid();
                          }
                          return;
                      },
                      [read_facade, &results_map](const qps::StmtSynonym& synonym, const qps::VarSynonym& var) {
                          // TODO: Improve pkb API: Get all statement that modifies and all variables that are modified
                          const auto statements = read_facade->get_all_statements();
                          const auto variables = read_facade->get_variables();
                          const auto stmt_vec = std::vector<std::string>{statements.begin(), statements.end()};
                          const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

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
                          const auto variables =
                              read_facade->get_vars_modified_by_statement(std::to_string(stmt_num.value));
                          results_map.update_mapping(var, variables);
                          return;
                      },
                      [read_facade, &results_map](const qps::StmtSynonym& synonym, const qps::QuotedIdent& identifier) {
                          const auto stmts = read_facade->get_statements_that_modify_var(identifier.get_value());

                          results_map.update_mapping(synonym, stmts);

                          return;
                      },
                      [read_facade, &results_map](const qps::Integer& stmt_num, const qps::QuotedIdent& identifier) {
                          const auto does_modify = read_facade->does_statement_modify_var(
                              std::to_string(stmt_num.value), identifier.get_value());

                          if (!does_modify) {
                              results_map.set_unsatisfiable();
                          } else {
                              // TODO: this no longer holds if we allow chaining of clauses
                              results_map.set_valid();
                          }
                          return;
                      },
                      [](const auto&) {
                          return std::unordered_set<std::string>{};
                      }};
};
}
