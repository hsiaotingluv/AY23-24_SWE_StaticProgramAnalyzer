#include "pkb/facades/read_facade.h"

#include <utility>

namespace pkb {
ReadFacade::ReadFacade(std::shared_ptr<PkbManager> pkb) : pkb(std::move(pkb)) {
}

std::unordered_set<std::string> ReadFacade::get_entities() const {
    return pkb->get_entities();
}

std::unordered_set<std::string> ReadFacade::get_procedures() const {
    return pkb->get_procedures();
}

std::unordered_set<std::string> ReadFacade::get_variables() const {
    return pkb->get_variables();
}

std::unordered_set<std::string> ReadFacade::get_constants() const {
    return pkb->get_constants();
}

std::unordered_set<std::string> ReadFacade::get_all_statements() const {
    return pkb->get_all_statements();
}

std::unordered_set<std::string> ReadFacade::get_assign_statements() const {
    return pkb->get_assign_statements();
}

std::unordered_set<std::string> ReadFacade::get_if_statements() const {
    return pkb->get_if_statements();
}

std::unordered_set<std::string> ReadFacade::get_while_statements() const {
    return pkb->get_while_statements();
}

std::unordered_set<std::string> ReadFacade::get_read_statements() const {
    return pkb->get_read_statements();
}

std::unordered_set<std::string> ReadFacade::get_print_statements() const {
    return pkb->get_print_statements();
}

std::unordered_set<std::string> ReadFacade::get_call_statements() const {
    return pkb->get_call_statements();
}

std::unordered_set<std::string> ReadFacade::get_vars_modified_by_statement(const std::string& s) const {
    return pkb->get_vars_modified_by_statement(s);
}

std::unordered_set<std::string> ReadFacade::get_statements_that_modify_var(const std::string& variable) const {
    return pkb->get_statements_that_modify_var(variable);
}

std::unordered_set<std::string> ReadFacade::get_statements_that_modify_var(const std::string& variable,
                                                                           const StatementType& statement_type) const {
    return pkb->get_statements_that_modify_var(variable, statement_type);
}

bool ReadFacade::has_statement_modify_var() const {
    return pkb->has_statement_modify_var();
}

bool ReadFacade::contains_statement_modify_var(const std::string& statement_number, const std::string& variable) const {
    return pkb->contains_statement_modify_var(statement_number, variable);
}

bool ReadFacade::contains_statement_modify_var_key(const std::string& statement_number) const {
    return pkb->contains_statement_modify_var_key(statement_number);
}

bool ReadFacade::contains_statement_modify_var_value(const std::string& variable) const {
    return pkb->contains_statement_modify_var_value(variable);
}

std::unordered_set<std::string> ReadFacade::get_all_statements_that_modify() const {
    return pkb->get_all_statements_that_modify();
}

std::unordered_set<std::string> ReadFacade::get_all_statements_that_modify(const StatementType& statement_type) const {
    return pkb->get_all_statements_that_modify(statement_type);
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_statements_and_var_modify_pairs() const {
    return pkb->get_all_statements_and_var_modify_pairs();
}

std::unordered_set<std::tuple<std::string, std::string>>
ReadFacade::get_all_statements_and_var_modify_pairs(const StatementType& statement_type) const {
    return pkb->get_all_statements_and_var_modify_pairs(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_all_procedures_that_modify() const {
    return pkb->get_all_procedures_that_modify();
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_procedures_and_var_modify_pairs() const {
    return pkb->get_all_procedures_and_var_modify_pairs();
}

std::unordered_set<std::string> ReadFacade::get_vars_modified_by_procedure(const std::string& procedure) const {
    return pkb->get_vars_modified_by_procedure(procedure);
}

std::unordered_set<std::string> ReadFacade::get_procedures_that_modify_var(const std::string& variable) const {
    return pkb->get_procedures_that_modify_var(variable);
}

bool ReadFacade::has_procedure_modify_var() const {
    return pkb->has_procedure_modify_var();
}

bool ReadFacade::contains_procedure_modify_var(const std::string& procedure, const std::string& variable) const {
    return pkb->contains_procedure_modify_var(procedure, variable);
}

bool ReadFacade::contains_procedure_modify_var_key(const std::string& procedure) const {
    return pkb->contains_procedure_modify_var_key(procedure);
}

bool ReadFacade::contains_procedure_modify_var_value(const std::string& variable) const {
    return pkb->contains_procedure_modify_var_value(variable);
}

std::unordered_set<std::string> ReadFacade::get_vars_used_by_statement(const std::string& s) const {
    return pkb->get_vars_used_by_statement(s);
}

std::unordered_set<std::string> ReadFacade::get_statements_that_use_var(const std::string& variable) const {
    return pkb->get_statements_that_use_var(variable);
}

std::unordered_set<std::string> ReadFacade::get_statements_that_use_var(const std::string& variable,
                                                                        const StatementType& statement_type) const {
    return pkb->get_statements_that_use_var(variable, statement_type);
}

bool ReadFacade::has_statement_use_var() const {
    return pkb->has_statement_use_var();
}

bool ReadFacade::contains_statement_use_var(const std::string& statement_number, const std::string& variable) const {
    return pkb->contains_statement_use_var(statement_number, variable);
}

bool ReadFacade::contains_statement_use_var_key(const std::string& statement_number) const {
    return pkb->contains_statement_use_var_key(statement_number);
}

bool ReadFacade::contains_statement_use_var_value(const std::string& variable) const {
    return pkb->contains_statement_use_var_value(variable);
}

std::unordered_set<std::string> ReadFacade::get_vars_used_by_procedure(const std::string& procedure) const {
    return pkb->get_vars_used_by_procedure(procedure);
}

std::unordered_set<std::string> ReadFacade::get_procedures_that_use_var(const std::string& variable) const {
    return pkb->get_procedures_that_use_var(variable);
}

bool ReadFacade::has_procedure_use_var() const {
    return pkb->has_procedure_use_var();
}

bool ReadFacade::contains_procedure_use_var(const std::string& procedure, const std::string& variable) const {
    return pkb->contains_procedure_use_var(procedure, variable);
}

bool ReadFacade::contains_procedure_use_var_key(const std::string& procedure) const {
    return pkb->contains_procedure_use_var_key(procedure);
}

bool ReadFacade::contains_procedure_use_var_value(const std::string& variable) const {
    return pkb->contains_procedure_use_var_value(variable);
}

std::unordered_set<std::string> ReadFacade::get_all_statements_that_use() const {
    return pkb->get_all_statements_that_use();
};

std::unordered_set<std::string> ReadFacade::get_all_statements_that_use(const StatementType& statement_type) const {
    return pkb->get_all_statements_that_use(statement_type);
};

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_statements_and_var_use_pairs() const {
    return pkb->get_all_statements_and_var_use_pairs();
}

std::unordered_set<std::tuple<std::string, std::string>>
ReadFacade::get_all_statements_and_var_use_pairs(const StatementType& statement_type) const {
    return pkb->get_all_statements_and_var_use_pairs(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_all_procedures_that_use() const {
    return pkb->get_all_procedures_that_use();
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_procedures_and_var_use_pairs() const {
    return pkb->get_all_procedures_and_var_use_pairs();
}

std::unordered_map<std::string, std::string> ReadFacade::get_all_follows() const {
    return pkb->get_all_follows();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_keys() const {
    return pkb->get_all_follows_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_keys(const StatementType& statement_type) const {
    return pkb->get_all_follows_keys(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_all_follows_values() const {
    return pkb->direct_follows_store->get_all_vals();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_values(const StatementType& statement_type) const {
    return pkb->get_all_follows_values(statement_type);
}

bool ReadFacade::has_follows_relation() const {
    return pkb->has_follows_relation();
}

bool ReadFacade::has_follows_relation(const std::string& stmt1, const std::string& stmt2) const {
    return pkb->has_follows_relation(stmt1, stmt2);
}

bool ReadFacade::contains_follows_key(const std::string& stmt) const {
    return pkb->contains_follows_key(stmt);
}

bool ReadFacade::contains_follows_value(const std::string& stmt) const {
    return pkb->contains_follows_value(stmt);
}

std::string ReadFacade::get_statement_following(const std::string& s) const {
    return pkb->get_statement_following(s);
}

std::string ReadFacade::get_statement_following(const std::string& s, const StatementType& statement_type) const {
    return pkb->get_statement_following(s, statement_type);
}

std::string ReadFacade::get_statement_followed_by(const std::string& s) const {
    return pkb->get_statement_followed_by(s);
}

std::string ReadFacade::get_statement_followed_by(const std::string& s, const StatementType& statement_type) const {
    return pkb->get_statement_followed_by(s, statement_type);
}

bool ReadFacade::has_follows_star_relation() const {
    return pkb->has_follows_star_relation();
}

bool ReadFacade::has_follows_star_relation(const std::string& stmt1, const std::string& stmt2) const {
    return pkb->has_follows_star_relation(stmt1, stmt2);
}

bool ReadFacade::contains_follows_star_key(const std::string& stmt) const {
    return pkb->contains_follows_star_key(stmt);
}

bool ReadFacade::contains_follows_star_value(const std::string& stmt) const {
    return pkb->contains_follows_star_value(stmt);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_follows_star() const {
    return pkb->get_all_follows_star();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_star_keys() const {
    return pkb->get_all_follows_star_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_star_keys(const StatementType& statement_type) const {
    return pkb->get_all_follows_star_keys(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_all_follows_star_values() const {
    return pkb->get_all_follows_star_values();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_star_values(const StatementType& statement_type) const {
    return pkb->get_all_follows_star_values(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_follows_stars_following(const std::string& stmt) const {
    return pkb->get_follows_stars_following(stmt);
}

std::unordered_set<std::string> ReadFacade::get_follows_stars_following(const std::string& stmt,
                                                                        const StatementType& statement_type) const {
    return pkb->get_follows_stars_following(stmt, statement_type);
}

std::unordered_set<std::string> ReadFacade::get_follows_stars_by(const std::string& stmt) const {
    return pkb->get_follows_stars_by(stmt);
}

std::unordered_set<std::string> ReadFacade::get_follows_stars_by(const std::string& stmt,
                                                                 const StatementType& statement_type) const {
    return pkb->get_follows_stars_by(stmt, statement_type);
}

bool ReadFacade::has_parent_relation() const {
    return pkb->has_parent_relation();
}

bool ReadFacade::has_parent_relation(const std::string& parent, const std::string& child) const {
    return pkb->has_parent_relation(parent, child);
}

bool ReadFacade::contains_parent_key(const std::string& parent) const {
    return pkb->contains_parent_key(parent);
}

bool ReadFacade::contains_parent_value(const std::string& child) const {
    return pkb->contains_parent_value(child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_parent() const {
    return pkb->get_all_parent();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_keys() const {
    return pkb->get_all_parent_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_keys(const StatementType& statement_type) const {
    return pkb->get_all_parent_keys(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_all_parent_values() const {
    return pkb->get_all_parent_values();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_values(const StatementType& statement_type) const {
    return pkb->get_all_parent_values(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_children_of(const std::string& parent) const {
    return pkb->get_children_of(parent);
}

std::unordered_set<std::string> ReadFacade::get_children_of(const std::string& parent,
                                                            const StatementType& statement_type) const {
    return pkb->get_children_of(parent, statement_type);
}

std::string ReadFacade::get_parent_of(const std::string& child) const {
    return pkb->get_parent_of(child);
}

std::string ReadFacade::get_parent_of(const std::string& child, const StatementType& statement_type) const {
    return pkb->get_parent_of(child, statement_type);
}

bool ReadFacade::has_parent_star_relation() const {
    return pkb->has_parent_star_relation();
}

bool ReadFacade::has_parent_star_relation(const std::string& parent, const std::string& child) const {
    return pkb->has_parent_star_relation(parent, child);
}

bool ReadFacade::contains_parent_star_key(const std::string& parent) const {
    return pkb->contains_parent_star_key(parent);
}

bool ReadFacade::contains_parent_star_value(const std::string& child) const {
    return pkb->contains_parent_star_value(child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_parent_star() const {
    return pkb->get_all_parent_star();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_star_keys() const {
    return pkb->get_all_parent_star_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_star_keys(const StatementType& statement_type) const {
    return pkb->get_all_parent_star_keys(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_all_parent_star_values() const {
    return pkb->get_all_parent_star_values();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_star_values(const StatementType& statement_type) const {
    return pkb->get_all_parent_star_values(statement_type);
}

std::unordered_set<std::string> ReadFacade::get_children_star_of(const std::string& parent) const {
    return pkb->get_children_star_of(parent);
}

std::unordered_set<std::string> ReadFacade::get_children_star_of(const std::string& parent,
                                                                 const StatementType& statement_type) const {
    return pkb->get_children_star_of(parent, statement_type);
}

std::unordered_set<std::string> ReadFacade::get_parent_star_of(const std::string& child) const {
    return pkb->get_parent_star_of(child);
}

std::unordered_set<std::string> ReadFacade::get_parent_star_of(const std::string& child,
                                                               const StatementType& statement_type) const {
    return pkb->get_parent_star_of(child, statement_type);
}

bool ReadFacade::has_next_relation() const {
    return pkb->has_next_relation();
}

bool ReadFacade::has_next_relation(const std::string& before, const std::string& after) const {
    return pkb->has_next_relation(before, after);
}

bool ReadFacade::contains_next_key(const std::string& before) const {
    return pkb->contains_next_key(before);
}

bool ReadFacade::contains_next_value(const std::string& after) const {
    return pkb->contains_next_value(after);
}

std::unordered_set<std::string> ReadFacade::get_all_next_keys() const {
    return pkb->get_all_next_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_next_values() const {
    return pkb->get_all_next_values();
}

std::unordered_set<std::string> ReadFacade::get_next_of(const std::string& before) const {
    return pkb->get_next_of(before);
}

std::unordered_set<std::string> ReadFacade::get_previous_of(const std::string& after) const {
    return pkb->get_previous_of(after);
}

bool ReadFacade::has_calls_relation() const {
    return pkb->has_calls_relation();
}

bool ReadFacade::has_calls_relation(const std::string& caller, const std::string& callee) const {
    return pkb->has_calls_relation(caller, callee);
}

bool ReadFacade::contains_calls_key(const std::string& caller) const {
    return pkb->contains_calls_key(caller);
}

bool ReadFacade::contains_calls_value(const std::string& callee) const {
    return pkb->contains_calls_value(callee);
}

std::unordered_set<std::string> ReadFacade::get_all_calls_callees() const {
    return pkb->get_all_calls_values();
}

std::unordered_set<std::string> ReadFacade::get_all_calls_callers() const {
    return pkb->get_all_calls_keys();
}

std::unordered_set<std::string> ReadFacade::get_callees(const std::string& caller) const {
    return pkb->get_callees(caller);
}

std::unordered_set<std::string> ReadFacade::get_callers(const std::string& callee) const {
    return pkb->get_callers(callee);
}

std::string ReadFacade::get_procedure_name_called_by(const std::string& stmt_number) const {
    return pkb->get_procedure_name_called_by(stmt_number);
}

bool ReadFacade::has_calls_star_relation() const {
    return pkb->has_calls_star_relation();
}

bool ReadFacade::has_calls_star_relation(const std::string& caller, const std::string& callee) const {
    return pkb->has_calls_star_relation(caller, callee);
}

bool ReadFacade::contains_calls_star_key(const std::string& caller) const {
    return pkb->contains_calls_star_key(caller);
}

bool ReadFacade::contains_calls_star_value(const std::string& callee) const {
    return pkb->contains_calls_star_value(callee);
}

std::unordered_set<std::string> ReadFacade::get_all_calls_star_values() const {
    return pkb->get_all_calls_star_values();
}

std::unordered_set<std::string> ReadFacade::get_all_calls_star_keys() const {
    return pkb->get_all_calls_star_keys();
}

std::unordered_set<std::string> ReadFacade::get_star_callees(const std::string& caller) const {
    return pkb->get_star_callees(caller);
}

std::unordered_set<std::string> ReadFacade::get_star_callers(const std::string& callee) const {
    return pkb->get_star_callers(callee);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_rhs(const std::string& rhs) const {
    return pkb->get_all_assignments_rhs(rhs);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_rhs_partial(const std::string& rhs) const {
    return pkb->get_all_assignments_rhs_partial(rhs);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_lhs(const std::string& lhs) const {
    return pkb->get_all_assignments_lhs(lhs);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_lhs_rhs(const std::string& lhs,
                                                                        const std::string& rhs) const {
    return pkb->get_all_assignments_lhs_rhs(lhs, rhs);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_lhs_rhs_partial(const std::string& lhs,
                                                                                const std::string& rhs) const {
    return pkb->get_all_assignments_lhs_rhs_partial(lhs, rhs);
}

std::unordered_set<std::string> ReadFacade::get_if_stmts_with_var() const {
    return pkb->get_if_stmts_with_var();
}

std::unordered_set<std::string> ReadFacade::get_if_stmts_with_var(const std::string& variable) const {
    return pkb->get_if_stmts_with_var(variable);
}

std::unordered_set<std::string> ReadFacade::get_vars_in_any_if() const {
    return pkb->get_vars_in_any_if();
}

std::unordered_set<std::string> ReadFacade::get_vars_in_if(const std::string& if_stmt) const {
    return pkb->get_vars_in_if(if_stmt);
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_if_stmt_var_pairs() const {
    return pkb->get_all_if_stmt_var_pairs();
}

std::unordered_set<std::string> ReadFacade::get_while_stmts_with_var() const {
    return pkb->get_while_stmts_with_var();
}

std::unordered_set<std::string> ReadFacade::get_while_stmts_with_var(const std::string& variable) const {
    return pkb->get_while_stmts_with_var(variable);
}

std::unordered_set<std::string> ReadFacade::get_vars_in_any_while() const {
    return pkb->get_vars_in_any_while();
}

std::unordered_set<std::string> ReadFacade::get_vars_in_while(const std::string& while_stmt) const {
    return pkb->get_vars_in_while(while_stmt);
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_while_stmt_var_pairs() const {
    return pkb->get_all_while_stmt_var_pairs();
}
} // namespace pkb
