#pragma once

#include "PKB/Facades/ReadFacade.h"
#include "qps/evaluators/results_map.hpp"
#include "qps/evaluators/type_traits.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <memory>

namespace qps {
const auto ModifiesS_evaluator = [](std::shared_ptr<ReadFacade> readFacade, ResultsMap& results_map) {
    return overloaded{[readFacade, &results_map](const qps::StmtSynonym& synonym, const qps::WildCard& wildcard2) {
                          // TODO: Improve PKB API: Get all statement that modifies
                          const auto variables = readFacade->getVariables();
                          const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

                          for (const auto& var : var_vec) {
                              const auto statements = readFacade->getStatementsThatModifyVar(var);
                              const auto stmt_vec = std::vector<std::string>{statements.begin(), statements.end()};

                              for (const auto& stmt : stmt_vec) {
                                  results_map.update_mapping(synonym, stmt);
                              }
                          }
                          return;
                      },
                      [readFacade, &results_map](const qps::Integer& stmt_num, const qps::WildCard& wildcard2) {
                          // TODO: Improve PKB API: Does statement modify var?
                          auto does_statement_modify_var = false;
                          const auto variables = readFacade->getVariables();
                          const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

                          for (const auto& var : var_vec) {
                              const auto statements = readFacade->getStatementsThatModifyVar(var);
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
                      [readFacade, &results_map](const qps::StmtSynonym& synonym, const qps::VarSynonym& var) {
                          // TODO: Improve PKB API: Get all statement that modifies and all variables that are modified
                          const auto statements = readFacade->getAllStatements();
                          const auto variables = readFacade->getVariables();
                          const auto stmt_vec = std::vector<std::string>{statements.begin(), statements.end()};
                          const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

                          for (const auto& stmt : stmt_vec) {
                              for (const auto& v : var_vec) {
                                  if (readFacade->doesStatementModifyVar(stmt, v)) {
                                      results_map.update_mapping(synonym, stmt);
                                      results_map.update_mapping(var, v);
                                  }
                              }
                          }
                          return;
                      },
                      [readFacade, &results_map](const qps::Integer& stmt_num, const qps::VarSynonym& var) {
                          const auto variables = readFacade->getVarsModifiedByStatement(std::to_string(stmt_num.value));
                          results_map.update_mapping(var, variables);
                          return;
                      },
                      [readFacade, &results_map](const qps::StmtSynonym& synonym, const qps::QuotedIdent& identifier) {
                          const auto stmts = readFacade->getStatementsThatModifyVar(identifier.get_value());

                          results_map.update_mapping(synonym, stmts);

                          return;
                      },
                      [readFacade, &results_map](const qps::Integer& stmt_num, const qps::QuotedIdent& identifier) {
                          const auto does_modify = readFacade->doesStatementModifyVar(std::to_string(stmt_num.value),
                                                                                      identifier.get_value());

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
}
