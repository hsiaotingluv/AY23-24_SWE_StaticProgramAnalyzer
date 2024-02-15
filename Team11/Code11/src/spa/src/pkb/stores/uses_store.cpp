#include "pkb/stores/uses_store.h"

UsesStore::UsesStore() = default;

void UsesStore::add_procedure_uses_var(const Procedure& p, const Variable& v) {
    procedure_uses_var_store[p].insert(v);
    reverse_procedure_uses_var_store[v].insert(p);
};

void UsesStore::add_statement_uses_var(const StatementNumber& s, const Variable& v) {
    statement_uses_var_store[s].insert(v);
    reverse_statement_uses_var_store[v].insert(s);
};

bool UsesStore::does_statement_use_var(const StatementNumber& s, const Variable& v) {
    if (statement_uses_var_store.find(s) == statement_uses_var_store.end()) {
        return false;
    }

    auto relevantSet = statement_uses_var_store.at(s);

    return relevantSet.find(v) != relevantSet.end();
}

bool UsesStore::does_procedure_use_var(const Procedure& p, const Variable& v) {
    if (procedure_uses_var_store.find(p) == procedure_uses_var_store.end()) {
        return false;
    }

    auto relevantSet = procedure_uses_var_store.at(p);

    return relevantSet.find(v) != relevantSet.end();
}

std::unordered_set<Variable> UsesStore::get_vars_used_by_statement(const StatementNumber& s) {
    if (statement_uses_var_store.find(s) == statement_uses_var_store.end()) {
        return std::unordered_set<Variable>{};
    }

    return statement_uses_var_store.at(s);
};

std::unordered_set<Variable> UsesStore::get_vars_used_by_procedure(const Procedure& p) {
    if (procedure_uses_var_store.find(p) == procedure_uses_var_store.end()) {
        return std::unordered_set<Variable>{};
    }

    return procedure_uses_var_store.at(p);
};

std::unordered_set<UsesStore::StatementNumber> UsesStore::get_statements_that_use_var(const Variable& v) {
    if (reverse_statement_uses_var_store.find(v) == reverse_statement_uses_var_store.end()) {
        return std::unordered_set<StatementNumber>{};
    }

    return reverse_statement_uses_var_store.at(v);
};

std::unordered_set<Procedure> UsesStore::get_procedures_that_use_var(const Variable& v) {
    if (reverse_procedure_uses_var_store.find(v) == reverse_procedure_uses_var_store.end()) {
        return std::unordered_set<Procedure>{};
    }

    return reverse_procedure_uses_var_store.at(v);
};

std::unordered_set<UsesStore::StatementNumber> UsesStore::get_all_statements_that_use() {
    std::unordered_set<StatementNumber> allStatements;

    for (const auto& [s, _] : statement_uses_var_store) {
        allStatements.insert(s);
    }

    return allStatements;
};

bool UsesStore::does_statement_use_any_var(const StatementNumber& statement) {
    return statement_uses_var_store.find(statement) != statement_uses_var_store.end();
}

UsesStore::StatementNumberVariableTupleSet UsesStore::get_all_statements_and_var_pairs() {
    StatementNumberVariableTupleSet allPairs;

    for (const auto& [s, vars] : statement_uses_var_store) {
        for (const auto& v : vars) {
            allPairs.insert(std::make_tuple(s, v));
        }
    }

    return allPairs;
};

std::unordered_set<Procedure> UsesStore::get_all_procedures_that_use() {
    std::unordered_set<Procedure> allProcedures;

    for (const auto& [p, _] : procedure_uses_var_store) {
        allProcedures.insert(p);
    }

    return allProcedures;
}

bool UsesStore::does_procedure_use_any_var(const Procedure& procedure) {
    return procedure_uses_var_store.find(procedure) != procedure_uses_var_store.end();
}

UsesStore::ProcedureVariableTupleSet UsesStore::get_all_procedures_and_var_pairs() {
    ProcedureVariableTupleSet allPairs;

    for (const auto& [p, vars] : procedure_uses_var_store) {
        for (const auto& v : vars) {
            allPairs.insert(std::make_tuple(p, v));
        }
    }

    return allPairs;
}
