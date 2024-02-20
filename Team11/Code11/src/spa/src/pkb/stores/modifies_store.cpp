#include "pkb/stores/modifies_store.h"

ModifiesStore::ModifiesStore() = default;

void ModifiesStore::add_procedure_modifies_var(const Procedure& p, const Variable& v) {
    procedure_modifies_var_store.add(p, v);
};

void ModifiesStore::add_statement_modifies_var(const StatementNumber& s, const Variable& v) {
    statement_modifies_var_store.add(s, v);
};

bool ModifiesStore::does_statement_modify_var(const StatementNumber& s, const Variable& v) {
    return statement_modifies_var_store.contains(s, v);
}

bool ModifiesStore::does_procedure_modify_var(const Procedure& p, const Variable& v) {
    return procedure_modifies_var_store.contains(p, v);
}

std::unordered_set<Variable> ModifiesStore::get_vars_modified_by_statement(const StatementNumber& s) {
    return statement_modifies_var_store.getValuesByKey(s);
};

std::unordered_set<Variable> ModifiesStore::get_vars_modified_by_procedure(const Procedure& p) {
    return procedure_modifies_var_store.getValuesByKey(p);
};

std::unordered_set<ModifiesStore::StatementNumber> ModifiesStore::get_statements_that_modify_var(const Variable& v) {
    return statement_modifies_var_store.getKeysByValue(v);
};

std::unordered_set<Procedure> ModifiesStore::get_procedures_that_modify_var(const Variable& v) {
    return procedure_modifies_var_store.getKeysByValue(v);
};

std::unordered_set<ModifiesStore::StatementNumber> ModifiesStore::get_all_statements_that_modify() {
    return statement_modifies_var_store.getAllKeys();
};

bool ModifiesStore::does_statement_modify_any_var(const StatementNumber& statement) {
    return statement_modifies_var_store.containsKey(statement);
};

ModifiesStore::StatementNumberVariableTupleSet ModifiesStore::get_all_statements_and_var_pairs() {
    return statement_modifies_var_store.getAllPairs();
};

std::unordered_set<Procedure> ModifiesStore::get_all_procedures_that_modify() {
    return procedure_modifies_var_store.getAllKeys();
};

bool ModifiesStore::does_procedure_modify_any_var(const Procedure& procedure) {
    return procedure_modifies_var_store.containsKey(procedure);
};

ModifiesStore::ProcedureVariableTupleSet ModifiesStore::get_all_procedures_and_var_pairs() {
    return procedure_modifies_var_store.getAllPairs();
};
