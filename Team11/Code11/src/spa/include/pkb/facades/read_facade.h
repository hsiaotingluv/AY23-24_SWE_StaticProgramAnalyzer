#pragma once

#include "pkb/common_types/constant.h"
#include "pkb/common_types/entity.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"
#include "pkb/pkb_manager.h"
#include <memory>
#include <tuple>
#include <unordered_set>

namespace pkb {
class ReadFacade {
  public:
    explicit ReadFacade(std::shared_ptr<PkbManager> pkb);

    // Entity-related Read Operations
    std::unordered_set<std::string> get_entities() const;

    std::unordered_set<std::string> get_procedures() const;

    std::unordered_set<std::string> get_variables() const;

    std::unordered_set<std::string> get_constants() const;

    bool has_entity(const std::string& entity) const;

    bool has_procedure(const std::string& procedure) const;

    bool has_variable(const std::string& variable) const;

    bool has_constant(const std::string& constant) const;

    // Statement-related Read Operations
    std::unordered_set<std::string> get_all_statements() const;

    std::unordered_set<std::string> get_assign_statements() const;

    std::unordered_set<std::string> get_if_statements() const;

    std::unordered_set<std::string> get_while_statements() const;

    std::unordered_set<std::string> get_read_statements() const;

    std::unordered_set<std::string> get_print_statements() const;

    std::unordered_set<std::string> get_call_statements() const;

    bool has_statement(const std::string& s) const;

    bool has_assign_statement(const std::string& s) const;

    bool has_if_statement(const std::string& s) const;

    bool has_while_statement(const std::string& s) const;

    bool has_read_statement(const std::string& s) const;

    bool has_print_statement(const std::string& s) const;

    bool has_call_statement(const std::string& s) const;

    // Modifies-related Read Operations
    std::unordered_set<std::string> get_vars_modified_by_statement(const std::string& s) const;

    std::unordered_set<std::string> get_statements_that_modify_var(const std::string& variable) const;

    std::unordered_set<std::string> get_statements_that_modify_var(const std::string& variable,
                                                                   const StatementType& statement_type) const;

    bool has_statement_modify_var() const;

    bool contains_statement_modify_var(const std::string& statement_number, const std::string& variable) const;

    bool contains_statement_modify_var_key(const std::string& statement_number) const;

    bool contains_statement_modify_var_value(const std::string& variable) const;

    std::unordered_set<std::string> get_vars_modified_by_procedure(const std::string& procedure) const;

    std::unordered_set<std::string> get_procedures_that_modify_var(const std::string& variable) const;

    bool has_procedure_modify_var() const;

    bool contains_procedure_modify_var(const std::string& procedure, const std::string& variable) const;

    bool contains_procedure_modify_var_key(const std::string& procedure) const;

    bool contains_procedure_modify_var_value(const std::string& variable) const;

    std::unordered_set<std::string> get_all_statements_that_modify() const;

    std::unordered_set<std::string> get_all_statements_that_modify(const StatementType& statement_type) const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_modify_pairs() const;

    std::unordered_set<std::tuple<std::string, std::string>>
    get_all_statements_and_var_modify_pairs(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_procedures_that_modify() const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_modify_pairs() const;

    // Uses-related Read Operations
    std::unordered_set<std::string> get_vars_used_by_statement(const std::string& s) const;

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable) const;

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable,
                                                                const StatementType& statement_type) const;

    bool has_statement_use_var() const;

    bool contains_statement_use_var(const std::string& statement_number, const std::string& variable) const;

    bool contains_statement_use_var_key(const std::string& statement_number) const;

    bool contains_statement_use_var_value(const std::string& variable) const;

    std::unordered_set<std::string> get_vars_used_by_procedure(const std::string& procedure) const;

    std::unordered_set<std::string> get_procedures_that_use_var(const std::string& variable) const;

    bool has_procedure_use_var() const;

    bool contains_procedure_use_var(const std::string& procedure, const std::string& variable) const;

    bool contains_procedure_use_var_key(const std::string& procedure) const;

    bool contains_procedure_use_var_value(const std::string& variable) const;

    std::unordered_set<std::string> get_all_statements_that_use() const;

    std::unordered_set<std::string> get_all_statements_that_use(const StatementType& statement_type) const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_use_pairs() const;

    std::unordered_set<std::tuple<std::string, std::string>>
    get_all_statements_and_var_use_pairs(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_procedures_that_use() const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_use_pairs() const;

    // Follows-related Read Operations
    bool has_follows_relation() const;

    bool has_follows_relation(const std::string& stmt1, const std::string& stmt2) const;

    bool contains_follows_key(const std::string& stmt) const;

    bool contains_follows_value(const std::string& stmt) const;

    std::unordered_map<std::string, std::string> get_all_follows() const;

    std::unordered_set<std::string> get_all_follows_keys() const;

    std::unordered_set<std::string> get_all_follows_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_follows_values() const;

    std::unordered_set<std::string> get_all_follows_values(const StatementType& statement_type) const;

    std::string get_statement_following(const std::string& s) const;

    std::string get_statement_following(const std::string& s, const StatementType& statement_type) const;

    std::string get_statement_followed_by(const std::string& s) const;

    std::string get_statement_followed_by(const std::string& s, const StatementType& statement_type) const;

    bool has_follows_star_relation() const;

    bool has_follows_star_relation(const std::string& stmt1, const std::string& stmt2) const;

    bool contains_follows_star_key(const std::string& stmt) const;

    bool contains_follows_star_value(const std::string& stmt) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_follows_star() const;

    std::unordered_set<std::string> get_all_follows_star_keys() const;

    std::unordered_set<std::string> get_all_follows_star_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_follows_star_values() const;

    std::unordered_set<std::string> get_all_follows_star_values(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_follows_stars_following(const std::string& stmt) const;

    std::unordered_set<std::string> get_follows_stars_following(const std::string& stmt,
                                                                const StatementType& statement_type) const;

    std::unordered_set<std::string> get_follows_stars_by(const std::string& stmt) const;

    std::unordered_set<std::string> get_follows_stars_by(const std::string& stmt,
                                                         const StatementType& statement_type) const;

    // Parent-related Read Operations
    bool has_parent_relation() const;

    bool has_parent_relation(const std::string& parent, const std::string& child) const;

    bool contains_parent_key(const std::string& parent) const;

    bool contains_parent_value(const std::string& child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_parent() const;

    std::unordered_set<std::string> get_all_parent_keys() const;

    std::unordered_set<std::string> get_all_parent_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_parent_values() const;

    std::unordered_set<std::string> get_all_parent_values(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_children_of(const std::string& parent) const;

    std::unordered_set<std::string> get_children_of(const std::string& parent,
                                                    const StatementType& statement_type) const;

    std::string get_parent_of(const std::string& child) const;

    std::string get_parent_of(const std::string& child, const StatementType& statement_type) const;

    bool has_parent_star_relation() const;

    bool has_parent_star_relation(const std::string& parent, const std::string& child) const;

    bool contains_parent_star_key(const std::string& parent) const;

    bool contains_parent_star_value(const std::string& child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_parent_star() const;

    std::unordered_set<std::string> get_all_parent_star_keys() const;

    std::unordered_set<std::string> get_all_parent_star_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_parent_star_values() const;

    std::unordered_set<std::string> get_all_parent_star_values(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_children_star_of(const std::string& parent) const;

    std::unordered_set<std::string> get_children_star_of(const std::string& parent,
                                                         const StatementType& statement_type) const;

    std::unordered_set<std::string> get_parent_star_of(const std::string& child) const;

    std::unordered_set<std::string> get_parent_star_of(const std::string& child,
                                                       const StatementType& statement_type) const;

    // Next-related Read Operations
    bool has_next_relation() const;

    bool has_next_relation(const std::string& before, const std::string& after) const;

    bool contains_next_key(const std::string& before) const;

    bool contains_next_value(const std::string& after) const;

    std::unordered_set<std::string> get_all_next_keys() const;

    std::unordered_set<std::string> get_all_next_values() const;

    std::unordered_set<std::string> get_next_of(const std::string& before) const;

    std::unordered_set<std::string> get_previous_of(const std::string& after) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_next() const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_next_reverse() const;

    // Calls-related Read Operations
    bool has_calls_relation() const;

    bool has_calls_relation(const std::string& caller, const std::string& callee) const;

    bool contains_calls_key(const std::string& caller) const;

    bool contains_calls_value(const std::string& callee) const;

    std::unordered_set<std::string> get_all_calls_callees() const;

    std::unordered_set<std::string> get_all_calls_callers() const;

    std::unordered_set<std::string> get_callees(const std::string& caller) const;

    std::unordered_set<std::string> get_callers(const std::string& callee) const;

    std::string get_procedure_name_called_by(const std::string& stmt_number) const;

    bool has_calls_star_relation() const;

    bool has_calls_star_relation(const std::string& caller, const std::string& callee) const;

    bool contains_calls_star_key(const std::string& caller) const;

    bool contains_calls_star_value(const std::string& callee) const;

    std::unordered_set<std::string> get_all_calls_star_values() const;

    std::unordered_set<std::string> get_all_calls_star_keys() const;

    std::unordered_set<std::string> get_star_callees(const std::string& caller) const;

    std::unordered_set<std::string> get_star_callers(const std::string& callee) const;

    // Assignment pattern-related Read Operations
    std::unordered_set<std::string> get_all_assignments_rhs(const std::string& rhs) const;

    std::unordered_set<std::string> get_all_assignments_rhs_partial(const std::string& rhs) const;

    std::unordered_set<std::string> get_all_assignments_lhs(const std::string& lhs) const;

    std::unordered_set<std::string> get_all_assignments_lhs_rhs(const std::string& lhs, const std::string& rhs) const;

    std::unordered_set<std::string> get_all_assignments_lhs_rhs_partial(const std::string& lhs,
                                                                        const std::string& rhs) const;

    // If pattern-related Read Operations
    std::unordered_set<std::string> get_if_stmts_with_var() const;

    std::unordered_set<std::string> get_if_stmts_with_var(const std::string& variable) const;

    std::unordered_set<std::string> get_vars_in_any_if() const;

    std::unordered_set<std::string> get_vars_in_if(const std::string& if_stmt) const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_if_stmt_var_pairs() const;

    // While pattern-related Read Operations
    std::unordered_set<std::string> get_while_stmts_with_var() const;

    std::unordered_set<std::string> get_while_stmts_with_var(const std::string& variable) const;

    std::unordered_set<std::string> get_vars_in_any_while() const;

    std::unordered_set<std::string> get_vars_in_while(const std::string& while_stmt) const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_while_stmt_var_pairs() const;

  private:
    std::shared_ptr<PkbManager> pkb;
};
} // namespace pkb
