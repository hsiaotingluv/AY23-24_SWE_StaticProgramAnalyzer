#include "PKB/Stores/ModifiesStore.h"

ModifiesStore::ModifiesStore() = default;

void ModifiesStore::addProcedureModifiesVar(const Procedure& p, const Variable& v) {
    procedureModifiesVarStore[p].insert(v);
    reverseProcedureModifiesVarStore[v].insert(p);
};

void ModifiesStore::addStatementModifiesVar(const StatementNumber& s, const Variable& v) {
    statementModifiesVarStore[s].insert(v);
    reverseStatementModifiesVarStore[v].insert(s);
};

bool ModifiesStore::doesStatementModifyVar(const StatementNumber& s, const Variable& v) {
    if (statementModifiesVarStore.find(s) == statementModifiesVarStore.end()) return false;

    auto relevantSet = statementModifiesVarStore.at(s);

    return relevantSet.find(v) != relevantSet.end();
}

bool ModifiesStore::doesProcedureModifyVar(const Procedure& p, const Variable& v) {
    if (procedureModifiesVarStore.find(p) == procedureModifiesVarStore.end()) return false;

    auto relevantSet = procedureModifiesVarStore.at(p);

    return relevantSet.find(v) != relevantSet.end();
}

std::unordered_set<Variable> ModifiesStore::getVarsModifiedByStatement(const StatementNumber& s) {
    if (statementModifiesVarStore.find(s) == statementModifiesVarStore.end()) return std::unordered_set<Variable>{};

    return statementModifiesVarStore.at(s);
};

std::unordered_set<Variable> ModifiesStore::getVarsModifiedByProcedure(const Procedure& p) {
    if (procedureModifiesVarStore.find(p) == procedureModifiesVarStore.end()) return std::unordered_set<Variable>{};

    return procedureModifiesVarStore.at(p);
};

std::unordered_set<ModifiesStore::StatementNumber> ModifiesStore::getStatementsThatModifyVar(const Variable& v) {
    if (reverseStatementModifiesVarStore.find(v) == reverseStatementModifiesVarStore.end()) {
        return std::unordered_set<StatementNumber>{};
    }

    return reverseStatementModifiesVarStore.at(v);
};

std::unordered_set<Procedure> ModifiesStore::getProceduresThatModifyVar(const Variable& v) {
    if (reverseProcedureModifiesVarStore.find(v) == reverseProcedureModifiesVarStore.end()) {
        return std::unordered_set<Procedure>{};
    }

    return reverseProcedureModifiesVarStore.at(v);
};


