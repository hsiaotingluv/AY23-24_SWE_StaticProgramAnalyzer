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

ModifiesStore::VariableSet ModifiesStore::get_vars_modified_by_statement(const StatementNumber& s) {
    return statement_modifies_var_store.getValuesByKey(s);
};

ModifiesStore::VariableSet ModifiesStore::get_vars_modified_by_procedure(const Procedure& p) {
    return procedure_modifies_var_store.getValuesByKey(p);
};

ModifiesStore::StatementSet ModifiesStore::get_statements_that_modify_var(const Variable& v) {
    return statement_modifies_var_store.getKeysByValue(v);
};

ModifiesStore::ProcedureSet ModifiesStore::get_procedures_that_modify_var(const Variable& v) {
    return procedure_modifies_var_store.getKeysByValue(v);
};

ModifiesStore::StatementSet ModifiesStore::get_all_statements_that_modify() {
    return statement_modifies_var_store.getAllKeys();
};

bool ModifiesStore::does_statement_modify_any_var(const StatementNumber& statement) {
    return statement_modifies_var_store.containsKey(statement);
};

ModifiesStore::StatementNumberVariableTupleSet ModifiesStore::get_all_statements_and_var_pairs() {
    return statement_modifies_var_store.getAllPairs();
};

ModifiesStore::ProcedureSet ModifiesStore::get_all_procedures_that_modify() {
    return procedure_modifies_var_store.getAllKeys();
};

bool ModifiesStore::does_procedure_modify_any_var(const Procedure& procedure) {
    return procedure_modifies_var_store.containsKey(procedure);
};

ModifiesStore::ProcedureVariableTupleSet ModifiesStore::get_all_procedures_and_var_pairs() {
    return procedure_modifies_var_store.getAllPairs();
};
