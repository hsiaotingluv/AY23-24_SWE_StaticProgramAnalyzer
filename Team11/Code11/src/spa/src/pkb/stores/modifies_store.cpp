#include "pkb/stores/modifies_store.h"

ModifiesStore::ModifiesStore() = default;

void ModifiesStore::add_procedure_modifies_var(const Procedure& p, const Variable& v) {
    procedure_modifies_var_store[p].insert(v);
    reverse_procedure_modifies_var_store[v].insert(p);
};

void ModifiesStore::add_statement_modifies_var(const StatementNumber& s, const Variable& v) {
    statement_modifies_var_store[s].insert(v);
    reverse_statement_modifies_var_store[v].insert(s);
};

bool ModifiesStore::does_statement_modify_var(const StatementNumber& s, const Variable& v) {
    if (statement_modifies_var_store.find(s) == statement_modifies_var_store.end()) {
        return false;
    }

    auto relevantSet = statement_modifies_var_store.at(s);

    return relevantSet.find(v) != relevantSet.end();
}

bool ModifiesStore::does_procedure_modify_var(const Procedure& p, const Variable& v) {
    if (procedure_modifies_var_store.find(p) == procedure_modifies_var_store.end()) {
        return false;
    }

    auto relevantSet = procedure_modifies_var_store.at(p);

    return relevantSet.find(v) != relevantSet.end();
}

std::unordered_set<Variable> ModifiesStore::get_vars_modified_by_statement(const StatementNumber& s) {
    if (statement_modifies_var_store.find(s) == statement_modifies_var_store.end()) {
        return std::unordered_set<Variable>{};
    }

    return statement_modifies_var_store.at(s);
};

std::unordered_set<Variable> ModifiesStore::get_vars_modified_by_procedure(const Procedure& p) {
    if (procedure_modifies_var_store.find(p) == procedure_modifies_var_store.end()) {
        return std::unordered_set<Variable>{};
    }

    return procedure_modifies_var_store.at(p);
};

std::unordered_set<ModifiesStore::StatementNumber> ModifiesStore::get_statements_that_modify_var(const Variable& v) {
    if (reverse_statement_modifies_var_store.find(v) == reverse_statement_modifies_var_store.end()) {
        return std::unordered_set<StatementNumber>{};
    }

    return reverse_statement_modifies_var_store.at(v);
};

std::unordered_set<Procedure> ModifiesStore::get_procedures_that_modify_var(const Variable& v) {
    if (reverse_procedure_modifies_var_store.find(v) == reverse_procedure_modifies_var_store.end()) {
        return std::unordered_set<Procedure>{};
    }

    return reverse_procedure_modifies_var_store.at(v);
};

std::unordered_set<ModifiesStore::StatementNumber> ModifiesStore::get_all_statements_that_modify() {
    std::unordered_set<StatementNumber> allStatements;

    for (const auto& [s, _] : statement_modifies_var_store) {
        allStatements.insert(s);
    }

    return allStatements;
};

bool ModifiesStore::does_statement_modify_any_var(const StatementNumber& statement) {
    return statement_modifies_var_store.find(statement) != statement_modifies_var_store.end();
};

ModifiesStore::StatementNumberVariableTupleSet ModifiesStore::get_all_statements_and_var_pairs() {
    StatementNumberVariableTupleSet allPairs;

    for (const auto& [s, vars] : statement_modifies_var_store) {
        for (const auto& v : vars) {
            allPairs.insert(std::make_tuple(s, v));
        }
    }

    return allPairs;
};

std::unordered_set<Procedure> ModifiesStore::get_all_procedures_that_modify() {
    std::unordered_set<Procedure> allProcedures;

    for (const auto& [p, _] : procedure_modifies_var_store) {
        allProcedures.insert(p);
    }

    return allProcedures;
};

bool ModifiesStore::does_procedure_modify_any_var(const Procedure& procedure) {
    return procedure_modifies_var_store.find(procedure) != procedure_modifies_var_store.end();
};

ModifiesStore::ProcedureVariableTupleSet ModifiesStore::get_all_procedures_and_var_pairs() {
    ProcedureVariableTupleSet allPairs;

    for (const auto& [p, vars] : procedure_modifies_var_store) {
        for (const auto& v : vars) {
            allPairs.insert(std::make_tuple(p, v));
        }
    }

    return allPairs;
};
