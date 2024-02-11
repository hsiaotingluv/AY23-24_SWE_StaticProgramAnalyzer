#pragma once

#include "PKB/CommonTypes/Procedure.h"
#include "PKB/CommonTypes/Variable.h"
#include <unordered_map>
#include <unordered_set>

class UsesStore {
  public:
    using StatementNumber = std::string;

    UsesStore();

    void addProcedureUsesVar(const Procedure& p, const Variable& v);
    void addStatementUsesVar(const StatementNumber& s, const Variable& v);

    bool doesProcedureUseVar(const Procedure& p, const Variable& v);
    bool doesStatementUseVar(const StatementNumber& s, const Variable& v);

    std::unordered_set<Variable> getVarsUsedByStatement(const StatementNumber& s);
    std::unordered_set<Variable> getVarsUsedByProcedure(const Procedure& p);

    std::unordered_set<StatementNumber> getStatementsThatUseVar(const Variable& v);
    std::unordered_set<Procedure> getProceduresThatUseVar(const Variable& v);

  private:
    std::unordered_map<Procedure, std::unordered_set<Variable>> procedureUsesVarStore;
    std::unordered_map<StatementNumber, std::unordered_set<Variable>> statementUsesVarStore;
    std::unordered_map<Variable, std::unordered_set<Procedure>> reverseProcedureUsesVarStore;
    std::unordered_map<Variable, std::unordered_set<StatementNumber>> reverseStatementUsesVarStore;
};