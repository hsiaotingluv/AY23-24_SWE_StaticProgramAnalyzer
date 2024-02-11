#pragma once
#include "pkb/CommonTypes.h"
#include "pkb/PKB.h"
#include <unordered_set>

class WriteFacade {
  public:
    typedef std::unordered_set<std::string> StringSet;
    typedef CommonTypes::STATEMENT_NUMBER StatementNumber;
    typedef CommonTypes::VARIABLE Variable;
    typedef CommonTypes::PROCEDURE Procedure;
    typedef CommonTypes::CONSTANT Constant;
    typedef CommonTypes::STATEMENT_TYPE StatementType;

    explicit WriteFacade(PKB& pkb);

    ~WriteFacade();

    void addProcedure(Procedure& procedure);

    void addVariable(Variable& variable);

    void addConstant(Constant& constant);

    void addStatement(StatementNumber statementNumber, StatementType statementType);

    void addStatementModifiesVar(StatementNumber statementNumber, Variable variable);

    void addProcedureModifiesvar(Procedure procedure, Variable variable);

  private:
    PKB& pkb;
};
