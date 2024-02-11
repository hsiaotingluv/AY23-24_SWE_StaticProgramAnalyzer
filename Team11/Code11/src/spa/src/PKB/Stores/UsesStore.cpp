#include "PKB/Stores/UsesStore.h"

UsesStore::UsesStore() = default;

void UsesStore::addProcedureUsesVar(const Procedure& p, const Variable& v) {
    procedureUsesVarStore[p].insert(v);
    reverseProcedureUsesVarStore[v].insert(p);
};

void UsesStore::addStatementUsesVar(const StatementNumber& s, const Variable& v) {
    statementUsesVarStore[s].insert(v);
    reverseStatementUsesVarStore[v].insert(s);
};

bool UsesStore::doesStatementUseVar(const StatementNumber& s, const Variable& v) {
    if (statementUsesVarStore.find(s) == statementUsesVarStore.end()) return false;

    auto relevantSet = statementUsesVarStore.at(s);

    return relevantSet.find(v) != relevantSet.end();
}

bool UsesStore::doesProcedureUseVar(const Procedure& p, const Variable& v) {
    if (procedureUsesVarStore.find(p) == procedureUsesVarStore.end()) return false;

    auto relevantSet = procedureUsesVarStore.at(p);

    return relevantSet.find(v) != relevantSet.end();
}

std::unordered_set<Variable> UsesStore::getVarsUsedByStatement(const StatementNumber& s) {
    if (statementUsesVarStore.find(s) == statementUsesVarStore.end()) return std::unordered_set<Variable>{};

    return statementUsesVarStore.at(s);
};

std::unordered_set<Variable> UsesStore::getVarsUsedByProcedure(const Procedure& p) {
    if (procedureUsesVarStore.find(p) == procedureUsesVarStore.end()) return std::unordered_set<Variable>{};

    return procedureUsesVarStore.at(p);
};

std::unordered_set<UsesStore::StatementNumber> UsesStore::getStatementsThatUseVar(const Variable& v) {
    if (reverseStatementUsesVarStore.find(v) == reverseStatementUsesVarStore.end()) {
        return std::unordered_set<StatementNumber>{};
    }

    return reverseStatementUsesVarStore.at(v);
};

std::unordered_set<Procedure> UsesStore::getProceduresThatUseVar(const Variable& v) {
    if (reverseProcedureUsesVarStore.find(v) == reverseProcedureUsesVarStore.end()) {
        return std::unordered_set<Procedure>{};
    }

    return reverseProcedureUsesVarStore.at(v);
};


