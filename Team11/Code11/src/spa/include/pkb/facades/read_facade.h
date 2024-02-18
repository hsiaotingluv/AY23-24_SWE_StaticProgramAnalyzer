#pragma once

#include "pkb/common_types/constant.h"
#include "pkb/common_types/entity.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/statement.h"
#include "pkb/common_types/variable.h"
#include "pkb/pkb.h"
#include <memory>
#include <tuple>
#include <unordered_set>

class ReadFacade {
  public:
    using StatementNumber = std::string;

    explicit ReadFacade(std::shared_ptr<PKB> pkb);

    // Entity-related Read Operations
    std::unordered_set<std::string> get_entities();

    std::unordered_set<std::string> get_procedures();

    std::unordered_set<std::string> get_variables();

    std::unordered_set<std::string> get_constants();

    // Statement-related Read Operations
    std::unordered_set<std::string> get_all_statements();

    std::unordered_set<std::string> get_raw_statements();

    std::unordered_set<std::string> get_assign_statements();

    std::unordered_set<std::string> get_if_statements();

    std::unordered_set<std::string> get_while_statements();

    std::unordered_set<std::string> get_read_statements();

    std::unordered_set<std::string> get_print_statements();

    std::unordered_set<std::string> get_call_statements();

    // Modifies-related Read Operations
    std::unordered_set<std::string> get_vars_modified_by_statement(const std::string& s);

    std::unordered_set<std::string> get_statements_that_modify_var(const std::string& variable);

    std::unordered_set<std::string> get_statements_that_modify_var(const std::string& variable,
                                                                   const StatementType& statementType);

    bool does_statement_modify_var(const std::string& statement_number, const std::string& variable);

    std::unordered_set<std::string> get_vars_modified_by_procedure(const std::string& procedure);

    std::unordered_set<std::string> get_procedures_that_modify_var(const std::string& variable);

    bool does_procedure_modify_var(const std::string& procedure, const std::string& variable);

    std::unordered_set<std::string> get_all_statements_that_modify();

    bool does_statement_modify_any_var(const std::string& statement_number);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_modify_pairs();

    std::unordered_set<std::string> get_all_procedures_that_modify();

    bool does_procedure_modify_any_var(const std::string& procedure);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_modify_pairs();

    // Uses-related Read Operations
    std::unordered_set<std::string> get_vars_used_by_statement(const std::string& s);

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable);

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable,
                                                                const StatementType& statementType);

    bool does_statement_use_var(const std::string& statement_number, const std::string& variable);

    std::unordered_set<std::string> get_vars_used_by_procedure(const std::string& procedure);

    std::unordered_set<std::string> get_procedures_that_use_var(const std::string& variable);

    bool does_procedure_use_var(const std::string& procedure, const std::string& variable);

    std::unordered_set<std::string> get_all_statements_that_use();

    bool does_statement_use_any_var(const std::string& statement_number);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_use_pairs();

    std::unordered_set<std::string> get_all_procedures_that_use();

    bool does_procedure_use_any_var(const std::string& procedure);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_use_pairs();

    // Follows-related Read Operations
    bool has_follows(const std::string& stmt1, const std::string& stmt2) const;

    std::unordered_map<std::string, std::string> get_all_follows() const;

    // Gets all statements followed by another statement from Follows relationship
    std::unordered_set<std::string> get_all_follows_keys() const;

    // Gets all statements following another statement from Follows relationship
    std::unordered_set<std::string> get_all_follows_values() const;

    std::string get_follows_following(const std::string& s) const;

    std::string get_follows_by(const std::string& s) const;

    bool has_follows_stars(const std::string& stmt1, const std::string& stmt2) const;

    // Gets all transitive Follows relationship
    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_follows_star() const;

    // Gets all statements followed by another statement from Follows relationship
    std::unordered_set<std::string> get_all_follows_star_keys() const;

    // Gets all statements following another statement from Follows relationship
    std::unordered_set<std::string> get_all_follows_star_values() const;

    std::unordered_set<std::string> get_follows_stars_following(const std::string& stmt) const;

    std::unordered_set<std::string> get_follows_stars_by(const std::string& stmt) const;

    // Parent-related Read Operations
    bool has_parent(const std::string& parent, const std::string& child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_parent() const;

    std::unordered_set<std::string> get_all_parent_keys() const;

    std::unordered_set<std::string> get_all_parent_values() const;

    std::unordered_set<std::string> get_parent_children(const std::string& parent) const;

    std::string get_parent(const std::string& child) const;

    bool has_parent_star(const std::string& parent, const std::string& child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_parent_star() const;

    std::unordered_set<std::string> get_all_parent_star_keys() const;

    std::unordered_set<std::string> get_all_parent_star_values() const;

    std::unordered_set<std::string> get_parent_star_children(const std::string& parent) const;

    std::unordered_set<std::string> get_star_parent(const std::string& child) const;

    // Next-related Read Operations
    bool has_next(const std::string& before, const std::string& after) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_next() const;

    std::unordered_set<std::string> get_all_next_keys() const;

    std::unordered_set<std::string> get_all_next_values() const;

    std::unordered_set<std::string> get_next(const std::string& before) const;

    std::unordered_set<std::string> get_next_previous(const std::string& after) const;

    bool has_next_star(const std::string& before, const std::string& after) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_next_star() const;

    std::unordered_set<std::string> get_all_next_star_keys() const;

    std::unordered_set<std::string> get_all_next_star_values() const;

    std::unordered_set<std::string> get_next_star(const std::string& before) const;

    std::unordered_set<std::string> get_next_star_previous(const std::string& after) const;

    // Calls-related Read Operations
    bool has_calls(const std::string& caller, const std::string& callee) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_calls() const;

    std::unordered_set<std::string> get_all_calls_values() const;

    std::unordered_set<std::string> get_all_calls_keys() const;

    std::unordered_set<std::string> get_callees(const std::string& caller) const;

    std::unordered_set<std::string> get_callers(const std::string& callee) const;

    bool has_calls_star(const std::string& caller, const std::string& callee) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_calls_star() const;

    std::unordered_set<std::string> get_all_calls_star_values() const;

    std::unordered_set<std::string> get_all_calls_star_keys() const;

    std::unordered_set<std::string> get_calls_star_callees(const std::string& caller) const;

    std::unordered_set<std::string> get_calls_star_callers(const std::string& callee) const;

    // Pattern-related Read Operations
    std::unordered_set<std::string> get_all_assignments_rhs(const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_rhs_partial(const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_lhs(const std::string& lhs);

    std::unordered_set<std::string> get_all_assignments_lhs_rhs(const std::string& lhs, const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_lhs_rhs_partial(const std::string& lhs, const std::string& rhs);

  private:
    std::shared_ptr<PKB> pkb;
};