#include "pkb/stores/uses_store.h"

UsesStore::UsesStore() = default;

void UsesStore::add_procedure_uses_var(const Procedure& p, const Variable& v) {
    procedure_uses_var_store.add(p, v);
};

void UsesStore::add_statement_uses_var(const StatementNumber& s, const Variable& v) {
    statement_uses_var_store.add(s, v);
};

bool UsesStore::does_statement_use_var(const StatementNumber& s, const Variable& v) {
    return statement_uses_var_store.contains(s, v);
}

bool UsesStore::does_procedure_use_var(const Procedure& p, const Variable& v) {
    return procedure_uses_var_store.contains(p, v);
}

UsesStore::VariableSet UsesStore::get_vars_used_by_statement(const StatementNumber& s) {
    return statement_uses_var_store.getValuesByKey(s);
};

UsesStore::VariableSet UsesStore::get_vars_used_by_procedure(const Procedure& p) {
    return procedure_uses_var_store.getValuesByKey(p);
};

UsesStore::StatementSet UsesStore::get_statements_that_use_var(const Variable& v) {
    return statement_uses_var_store.getKeysByValue(v);
};

UsesStore::ProcedureSet UsesStore::get_procedures_that_use_var(const Variable& v) {
    return procedure_uses_var_store.getKeysByValue(v);
};

UsesStore::StatementSet UsesStore::get_all_statements_that_use() {
    return statement_uses_var_store.getAllKeys();
};

bool UsesStore::does_statement_use_any_var(const StatementNumber& s) {
    return statement_uses_var_store.containsKey(s);
}

UsesStore::StatementNumberVariableTupleSet UsesStore::get_all_statements_and_var_pairs() {
    return statement_uses_var_store.getAllPairs();
};

UsesStore::ProcedureSet UsesStore::get_all_procedures_that_use() {
    return procedure_uses_var_store.getAllKeys();
}

bool UsesStore::does_procedure_use_any_var(const Procedure& p) {
    return procedure_uses_var_store.containsKey(p);
}

UsesStore::ProcedureVariableTupleSet UsesStore::get_all_procedures_and_var_pairs() {
    return procedure_uses_var_store.getAllPairs();
}
