#include "pkb/facades/read_facade.h"

#include <utility>

ReadFacade::ReadFacade(std::shared_ptr<PKB> pkb) : pkb(std::move(pkb)) {
}

std::unordered_set<std::string> ReadFacade::get_entities() {
    std::unordered_set<std::string> entities;

    auto procedures = this->pkb->entity_store->get_procedures();
    auto vars = this->pkb->entity_store->get_variables();
    auto consts = this->pkb->entity_store->get_constants();

    for (const Procedure& p : procedures) {
        entities.insert(p.getName());
    }
    for (const Variable& v : vars) {
        entities.insert(v.getName());
    }
    for (const Constant& c : consts) {
        entities.insert(c.getName());
    }

    return entities;
}

std::unordered_set<std::string> ReadFacade::get_procedures() {
    auto procedures = this->pkb->entity_store->get_procedures();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_variables() {
    auto vars = this->pkb->entity_store->get_variables();

    std::unordered_set<std::string> temp;
    for (const Variable& v : vars) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_constants() {
    auto consts = this->pkb->entity_store->get_constants();

    std::unordered_set<std::string> temp;
    for (const Constant& c : consts) {
        temp.insert(c.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_all_statements() {
    return this->pkb->statement_store->get_statements();
}

std::unordered_set<std::string> ReadFacade::get_raw_statements() {
    return this->pkb->statement_store->get_statements_of_type(StatementType::Raw);
}

std::unordered_set<std::string> ReadFacade::get_assign_statements() {
    return this->pkb->statement_store->get_statements_of_type(StatementType::Assign);
}

std::unordered_set<std::string> ReadFacade::get_if_statements() {
    return this->pkb->statement_store->get_statements_of_type(StatementType::If);
}

std::unordered_set<std::string> ReadFacade::get_while_statements() {
    return this->pkb->statement_store->get_statements_of_type(StatementType::While);
}

std::unordered_set<std::string> ReadFacade::get_read_statements() {
    return this->pkb->statement_store->get_statements_of_type(StatementType::Read);
}

std::unordered_set<std::string> ReadFacade::get_print_statements() {
    return this->pkb->statement_store->get_statements_of_type(StatementType::Print);
}

std::unordered_set<std::string> ReadFacade::get_call_statements() {
    return this->pkb->statement_store->get_statements_of_type(StatementType::Call);
}

std::unordered_set<std::string> ReadFacade::get_vars_modified_by_statement(const std::string& s) {
    auto variables = this->pkb->modifies_store->get_vars_modified_by_statement(s);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_statements_that_modify_var(const std::string& variable) {
    auto v = Variable(variable);

    return this->pkb->modifies_store->get_statements_that_modify_var(v);
}

std::unordered_set<std::string> ReadFacade::get_statements_that_modify_var(const std::string& variable,
                                                                           const StatementType& statementType) {
    auto stmts_pool = get_statements_that_modify_var(variable);

    std::unordered_set<std::string> temp;

    for (const std::string& stmt : stmts_pool) {
        if (this->pkb->statement_store->get_statement_type(stmt) == statementType) {
            temp.insert(stmt);
        }
    }

    return temp;
}

bool ReadFacade::does_statement_modify_var(const std::string& statement, const std::string& variable) {
    auto v = Variable(variable);

    return this->pkb->modifies_store->does_statement_modify_var(statement, v);
}

std::unordered_set<std::string> ReadFacade::get_all_statements_that_modify() {
    return this->pkb->modifies_store->get_all_statements_that_modify();
};

bool ReadFacade::does_statement_modify_any_var(const std::string& statement_number) {
    return this->pkb->modifies_store->does_statement_modify_any_var(statement_number);
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_statements_and_var_modify_pairs() {
    auto pairs = this->pkb->modifies_store->get_all_statements_and_var_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [s, v] : pairs) {
        temp.insert(std::make_tuple(s, v.getName()));
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_all_procedures_that_modify() {
    auto procedures = this->pkb->modifies_store->get_all_procedures_that_modify();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool ReadFacade::does_procedure_modify_any_var(const std::string& procedure) {
    auto p = Procedure(procedure);

    return this->pkb->modifies_store->does_procedure_modify_any_var(p);
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_procedures_and_var_modify_pairs() {
    auto pairs = this->pkb->modifies_store->get_all_procedures_and_var_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [p, v] : pairs) {
        temp.insert(std::make_tuple(p.getName(), v.getName()));
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_vars_modified_by_procedure(const std::string& procedure) {
    auto p = Procedure(procedure);

    auto variables = this->pkb->modifies_store->get_vars_modified_by_procedure(p);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_procedures_that_modify_var(const std::string& variable) {
    auto v = Variable(variable);

    auto procedures = this->pkb->modifies_store->get_procedures_that_modify_var(v);

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool ReadFacade::does_procedure_modify_var(const std::string& procedure, const std::string& variable) {
    auto v = Variable(variable);
    auto p = Procedure(procedure);

    return this->pkb->modifies_store->does_procedure_modify_var(p, v);
}

std::unordered_set<std::string> ReadFacade::get_vars_used_by_statement(const std::string& s) {
    auto variables = this->pkb->uses_store->get_vars_used_by_statement(s);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_statements_that_use_var(const std::string& variable) {
    auto v = Variable(variable);

    return this->pkb->uses_store->get_statements_that_use_var(v);
}

std::unordered_set<std::string> ReadFacade::get_statements_that_use_var(const std::string& variable,
                                                                        const StatementType& statementType) {
    auto stmts_pool = get_statements_that_use_var(variable);

    std::unordered_set<std::string> temp;

    for (const std::string& stmt : stmts_pool) {
        if (this->pkb->statement_store->get_statement_type(stmt) == statementType) {
            temp.insert(stmt);
        }
    }

    return temp;
}

bool ReadFacade::does_statement_use_var(const std::string& statement, const std::string& variable) {
    auto v = Variable(variable);

    return this->pkb->uses_store->does_statement_use_var(statement, v);
}

std::unordered_set<std::string> ReadFacade::get_vars_used_by_procedure(const std::string& procedure) {
    auto p = Procedure(procedure);

    auto variables = this->pkb->uses_store->get_vars_used_by_procedure(p);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_procedures_that_use_var(const std::string& variable) {
    auto v = Variable(variable);

    auto procedures = this->pkb->uses_store->get_procedures_that_use_var(v);

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool ReadFacade::does_procedure_use_var(const std::string& procedure, const std::string& variable) {
    auto v = Variable(variable);
    auto p = Procedure(procedure);

    return this->pkb->uses_store->does_procedure_use_var(p, v);
}

std::unordered_set<std::string> ReadFacade::get_all_statements_that_use() {
    return this->pkb->uses_store->get_all_statements_that_use();
};

bool ReadFacade::does_statement_use_any_var(const std::string& statement_number) {
    return this->pkb->uses_store->does_statement_use_any_var(statement_number);
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_statements_and_var_use_pairs() {
    auto pairs = this->pkb->uses_store->get_all_statements_and_var_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [s, v] : pairs) {
        temp.insert(std::make_tuple(s, v.getName()));
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::get_all_procedures_that_use() {
    auto procedures = this->pkb->uses_store->get_all_procedures_that_use();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool ReadFacade::does_procedure_use_any_var(const std::string& procedure) {
    auto p = Procedure(procedure);

    return this->pkb->uses_store->does_procedure_use_any_var(p);
}

std::unordered_set<std::tuple<std::string, std::string>> ReadFacade::get_all_procedures_and_var_use_pairs() {
    auto pairs = this->pkb->uses_store->get_all_procedures_and_var_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [p, v] : pairs) {
        temp.insert(std::make_tuple(p.getName(), v.getName()));
    }

    return temp;
}

std::unordered_map<std::string, std::string> ReadFacade::get_all_follows() const {
    return this->pkb->follows_store->get_all_follows();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_keys() const {
    return this->pkb->follows_store->get_all_follows_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_keys(const StatementType& statementType) const {
    auto stmts_pool = this->pkb->follows_store->get_all_follows_keys();
    return this->pkb->filterStatementsByType(stmts_pool, statementType);
}

std::unordered_set<std::string> ReadFacade::get_all_follows_values() const {
    return this->pkb->follows_store->get_all_follows_values();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_values(const StatementType& statementType) const {
    auto stmts_pool = this->pkb->follows_store->get_all_follows_values();
    return this->pkb->filterStatementsByType(stmts_pool, statementType);
}

bool ReadFacade::has_follows(const std::string& stmt1, const std::string& stmt2) const {
    return this->pkb->follows_store->has_follows(stmt1, stmt2);
}

std::string ReadFacade::get_follows_following(const std::string& s) const {
    return this->pkb->follows_store->get_follows_following(s);
}

std::string ReadFacade::get_follows_following(const std::string& s, const StatementType& statementType) const {
    auto stmt = this->pkb->follows_store->get_follows_following(s);

    if (this->pkb->statement_store->get_statement_type(stmt) == statementType) {
        return stmt;
    }

    return "";
}

std::string ReadFacade::get_follows_by(const std::string& s) const {
    return this->pkb->follows_store->get_follows_by(s);
}

std::string ReadFacade::get_follows_by(const std::string& s, const StatementType& statementType) const {
    auto stmt = this->pkb->follows_store->get_follows_by(s);

    if (this->pkb->statement_store->get_statement_type(stmt) == statementType) {
        return stmt;
    }

    return "";
}

bool ReadFacade::has_follows_stars(const std::string& stmt1, const std::string& stmt2) const {
    return this->pkb->follows_store->has_follows_stars(stmt1, stmt2);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_follows_star() const {
    return this->pkb->follows_store->get_all_follows_star();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_star_keys() const {
    return this->pkb->follows_store->get_all_follows_star_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_star_keys(const StatementType& statementType) const {
    auto stmts_pool = this->pkb->follows_store->get_all_follows_star_keys();
    return this->pkb->filterStatementsByType(stmts_pool, statementType);
}

std::unordered_set<std::string> ReadFacade::get_all_follows_star_values() const {
    return this->pkb->follows_store->get_all_follows_star_values();
}

std::unordered_set<std::string> ReadFacade::get_all_follows_star_values(const StatementType& statementType) const {
    auto stmts_pool = this->pkb->follows_store->get_all_follows_star_values();
    return this->pkb->filterStatementsByType(stmts_pool, statementType);
}

std::unordered_set<std::string> ReadFacade::get_follows_stars_following(const std::string& stmt) const {
    return this->pkb->follows_store->get_follows_stars_following(stmt);
}

std::unordered_set<std::string> ReadFacade::get_follows_stars_following(const std::string& stmt,
                                                                        const StatementType& statementType) const {
    auto stmts_pool = this->pkb->follows_store->get_follows_stars_following(stmt);
    return this->pkb->filterStatementsByType(stmts_pool, statementType);
}

std::unordered_set<std::string> ReadFacade::get_follows_stars_by(const std::string& stmt) const {
    return this->pkb->follows_store->get_follows_stars_by(stmt);
}

std::unordered_set<std::string> ReadFacade::get_follows_stars_by(const std::string& stmt,
                                                                 const StatementType& statementType) const {
    auto stmts_pool = this->pkb->follows_store->get_follows_stars_by(stmt);
    return this->pkb->filterStatementsByType(stmts_pool, statementType);
}

bool ReadFacade::has_parent(const std::string& parent, const std::string& child) const {
    return this->pkb->parent_store->has_parent(parent, child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_parent() const {
    return this->pkb->parent_store->get_all_parent();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_keys() const {
    return this->pkb->parent_store->get_all_parent_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_values() const {
    return this->pkb->parent_store->get_all_parent_values();
}

std::unordered_set<std::string> ReadFacade::get_parent_children(const std::string& parent) const {
    return this->pkb->parent_store->get_parent_children(parent);
}

std::string ReadFacade::get_parent(const std::string& child) const {
    return this->pkb->parent_store->get_parent(child);
}

bool ReadFacade::has_parent_star(const std::string& parent, const std::string& child) const {
    return this->pkb->parent_store->has_parent_star(parent, child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_parent_star() const {
    return this->pkb->parent_store->get_all_parent_star();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_star_keys() const {
    return this->pkb->parent_store->get_all_parent_star_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_parent_star_values() const {
    return this->pkb->parent_store->get_all_parent_star_values();
}

std::unordered_set<std::string> ReadFacade::get_parent_star_children(const std::string& parent) const {
    return this->pkb->parent_store->get_parent_star_children(parent);
}

std::unordered_set<std::string> ReadFacade::get_star_parent(const std::string& child) const {
    return this->pkb->parent_store->get_star_parent(child);
}

bool ReadFacade::has_next(const std::string& before, const std::string& after) const {
    return this->pkb->next_store->has_next(before, after);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_next() const {
    return this->pkb->next_store->get_all_next();
}

std::unordered_set<std::string> ReadFacade::get_all_next_keys() const {
    return this->pkb->next_store->get_all_next_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_next_values() const {
    return this->pkb->next_store->get_all_next_values();
}

std::unordered_set<std::string> ReadFacade::get_next(const std::string& before) const {
    return this->pkb->next_store->get_next(before);
}

std::unordered_set<std::string> ReadFacade::get_next_previous(const std::string& after) const {
    return this->pkb->next_store->get_next_previous(after);
}

bool ReadFacade::has_next_star(const std::string& before, const std::string& after) const {
    return this->pkb->next_store->has_next_star(before, after);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_next_star() const {
    return this->pkb->next_store->get_all_next_star();
}

std::unordered_set<std::string> ReadFacade::get_all_next_star_keys() const {
    return this->pkb->next_store->get_all_next_star_keys();
}

std::unordered_set<std::string> ReadFacade::get_all_next_star_values() const {
    return this->pkb->next_store->get_all_next_star_values();
}

std::unordered_set<std::string> ReadFacade::get_next_star(const StatementNumber& before) const {
    return this->pkb->next_store->get_next_star(before);
}

std::unordered_set<std::string> ReadFacade::get_next_star_previous(const StatementNumber& after) const {
    return this->pkb->next_store->get_next_star_previous(after);
}

bool ReadFacade::has_calls(const std::string& caller, const std::string& callee) const {
    return this->pkb->calls_store->has_calls(caller, callee);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_calls() const {
    return this->pkb->calls_store->get_all_calls();
}

std::unordered_set<std::string> ReadFacade::get_all_calls_values() const {
    return this->pkb->calls_store->get_all_calls_values();
}

std::unordered_set<std::string> ReadFacade::get_all_calls_keys() const {
    return this->pkb->calls_store->get_all_calls_keys();
}

std::unordered_set<std::string> ReadFacade::get_callees(const std::string& caller) const {
    return this->pkb->calls_store->get_callees(caller);
}

std::unordered_set<std::string> ReadFacade::get_callers(const std::string& callee) const {
    return this->pkb->calls_store->get_callers(callee);
}

bool ReadFacade::has_calls_star(const std::string& caller, const std::string& callee) const {
    return this->pkb->calls_store->has_calls_star(caller, callee);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::get_all_calls_star() const {
    return this->pkb->calls_store->get_all_calls_star();
}

std::unordered_set<std::string> ReadFacade::get_all_calls_star_values() const {
    return this->pkb->calls_store->get_all_calls_star_values();
}

std::unordered_set<std::string> ReadFacade::get_all_calls_star_keys() const {
    return this->pkb->calls_store->get_all_calls_star_keys();
}

std::unordered_set<std::string> ReadFacade::get_calls_star_callees(const std::string& caller) const {
    return this->pkb->calls_store->get_calls_star_callees(caller);
}

std::unordered_set<std::string> ReadFacade::get_calls_star_callers(const std::string& callee) const {
    return this->pkb->calls_store->get_calls_star_callers(callee);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_rhs(const std::string& rhs) {
    return this->pkb->assignment_store->get_all_assignments_rhs(rhs);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_rhs_partial(const std::string& rhs) {
    return this->pkb->assignment_store->get_all_assignments_rhs_partial(rhs);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_lhs(const std::string& lhs) {
    auto v = Variable(lhs);

    return this->pkb->assignment_store->get_all_assignments_lhs(v);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_lhs_rhs(const std::string& lhs,
                                                                        const std::string& rhs) {
    auto v = Variable(lhs);

    return this->pkb->assignment_store->get_all_assignments_lhs_rhs(v, rhs);
}

std::unordered_set<std::string> ReadFacade::get_all_assignments_lhs_rhs_partial(const std::string& lhs,
                                                                                const std::string& rhs) {
    auto v = Variable(lhs);

    return this->pkb->assignment_store->get_all_assignments_lhs_rhs_partial(v, rhs);
}