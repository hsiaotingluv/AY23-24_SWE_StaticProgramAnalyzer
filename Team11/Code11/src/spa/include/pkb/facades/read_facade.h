#pragma once

#include "pkb/common_types/constant.h"
#include "pkb/common_types/entity.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"
#include "pkb/pkb.h"
#include <memory>
#include <tuple>
#include <unordered_set>

class ReadFacade {
  public:
    explicit ReadFacade(std::shared_ptr<PKB> pkb);

    // Entity-related Read Operations
    std::unordered_set<std::string> get_entities();

    std::unordered_set<std::string> get_procedures();

    std::unordered_set<std::string> get_variables();

    std::unordered_set<std::string> get_constants();

    // Statement-related Read Operations
    std::unordered_set<std::string> get_all_statements();

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
                                                                   const StatementType& statement_type);

    bool does_statement_modify_var(const std::string& statement_number, const std::string& variable);

    std::unordered_set<std::string> get_vars_modified_by_procedure(const std::string& procedure);

    std::unordered_set<std::string> get_procedures_that_modify_var(const std::string& variable);

    bool does_procedure_modify_var(const std::string& procedure, const std::string& variable);

    std::unordered_set<std::string> get_all_statements_that_modify();

    std::unordered_set<std::string> get_all_statements_that_modify(const StatementType& statement_type);

    bool does_statement_modify_any_var(const std::string& statement_number);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_modify_pairs();

    std::unordered_set<std::tuple<std::string, std::string>>
    get_all_statements_and_var_modify_pairs(const StatementType& statement_type);

    std::unordered_set<std::string> get_all_procedures_that_modify();

    bool does_procedure_modify_any_var(const std::string& procedure);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_modify_pairs();

    // Uses-related Read Operations
    std::unordered_set<std::string> get_vars_used_by_statement(const std::string& s);

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable);

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable,
                                                                const StatementType& statement_type);

    bool does_statement_use_var(const std::string& statement_number, const std::string& variable);

    std::unordered_set<std::string> get_vars_used_by_procedure(const std::string& procedure);

    std::unordered_set<std::string> get_procedures_that_use_var(const std::string& variable);

    bool does_procedure_use_var(const std::string& procedure, const std::string& variable);

    std::unordered_set<std::string> get_all_statements_that_use();

    std::unordered_set<std::string> get_all_statements_that_use(const StatementType& statement_type);

    bool does_statement_use_any_var(const std::string& statement_number);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_use_pairs();

    std::unordered_set<std::tuple<std::string, std::string>>
    get_all_statements_and_var_use_pairs(const StatementType& statement_type);

    std::unordered_set<std::string> get_all_procedures_that_use();

    bool does_procedure_use_any_var(const std::string& procedure);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_use_pairs();

    // Follows-related Read Operations
    bool has_follows_relation(const std::string& stmt1, const std::string& stmt2) const;

    std::unordered_map<std::string, std::string> get_all_follows() const;

    std::unordered_set<std::string> get_all_follows_keys() const;

    std::unordered_set<std::string> get_all_follows_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_follows_values() const;

    std::unordered_set<std::string> get_all_follows_values(const StatementType& statement_type) const;

    std::string get_statement_following(const std::string& s) const;

    std::string get_statement_following(const std::string& s, const StatementType& statement_type) const;

    std::string get_statement_followed_by(const std::string& s) const;

    std::string get_statement_followed_by(const std::string& s, const StatementType& statement_type) const;

    bool has_follows_star_relation(const std::string& stmt1, const std::string& stmt2) const;

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
    bool has_parent_relation(const std::string& parent, const std::string& child) const;

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

    bool has_parent_star_relation(const std::string& parent, const std::string& child) const;

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

    // Pattern-related Read Operations
    std::unordered_set<std::string> get_all_assignments_rhs(const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_rhs_partial(const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_lhs(const std::string& lhs);

    std::unordered_set<std::string> get_all_assignments_lhs_rhs(const std::string& lhs, const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_lhs_rhs_partial(const std::string& lhs, const std::string& rhs);

  private:
    std::shared_ptr<PKB> pkb;
};