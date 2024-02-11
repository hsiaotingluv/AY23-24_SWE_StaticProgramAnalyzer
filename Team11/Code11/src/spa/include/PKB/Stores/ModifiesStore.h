#pragma once

#include "PKB/CommonTypes/Procedure.h"
#include "PKB/CommonTypes/Variable.h"
#include <unordered_map>
#include <unordered_set>

class ModifiesStore {
  public:
    using StatementNumber = std::string;

    ModifiesStore();

    void addProcedureModifiesVar(const Procedure& p, const Variable& v);
    void addStatementModifiesVar(const StatementNumber& s, const Variable& v);

    bool doesProcedureModifyVar(const Procedure& p, const Variable& v);
    bool doesStatementModifyVar(const StatementNumber& s, const Variable& v);

    std::unordered_set<Variable> getVarsModifiedByStatement(const StatementNumber& s);
    std::unordered_set<Variable> getVarsModifiedByProcedure(const Procedure& p);

    std::unordered_set<StatementNumber> getStatementsThatModifyVar(const Variable& v);
    std::unordered_set<Procedure> getProceduresThatModifyVar(const Variable& v);

  private:
    std::unordered_map<Procedure, std::unordered_set<Variable>> procedureModifiesVarStore;
    std::unordered_map<StatementNumber, std::unordered_set<Variable>> statementModifiesVarStore;
    std::unordered_map<Variable, std::unordered_set<Procedure>> reverseProcedureModifiesVarStore;
    std::unordered_map<Variable, std::unordered_set<StatementNumber>> reverseStatementModifiesVarStore;
};