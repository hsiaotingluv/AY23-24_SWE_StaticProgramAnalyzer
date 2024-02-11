#pragma once
#include "pkb/CommonTypes.h"
#include "pkb/PKB.h"
#include <unordered_set>

class ReadFacade {
  public:
    typedef std::unordered_set<std::string> StringSet;
    typedef CommonTypes::STATEMENT_NUMBER StatementNumber;
    typedef CommonTypes::VARIABLE Variable;
    typedef CommonTypes::PROCEDURE Procedure;

    explicit ReadFacade(PKB& pkb);

    ~ReadFacade();

    // Entity-related Read Operations
    StringSet getEntities();

    StringSet getProcedures();

    StringSet getVariables();

    StringSet getConstants();

    // Statement-related Read Operations
    StringSet getStatements();

    StringSet getAssignStatements();

    StringSet getIfStatements();

    StringSet getWhileStatements();

    StringSet getReadStatements();

    StringSet getPrintStatements();

    StringSet getCallStatements();

    // Modifies-related Read Operations
    StringSet getVarsModifiedByStatement(StatementNumber statement_number);

    StringSet getStatementsThatModifyVar(Variable variable);

    bool doesStatementModifyVar(StatementNumber statement_number, Variable variable);

    StringSet getVarsModifiedByProcedure(Procedure procedure);

    StringSet getProceduresThatModifyVar(Variable variable);

    bool doesProcedureModifyVar(Procedure procedure, Variable variable);

    // Uses-related Read Operations
    StringSet getVarsUsedByStatement(StatementNumber statement_number);

    StringSet getStatementsThatUsesVar(Variable variable);

    bool doesStatementUseVar(StatementNumber statement_number, Variable variable);

    StringSet getVarsUsedByProcedure(Procedure procedure);

    StringSet getProceduresThatUseVar(Variable variable);

    bool doesProcedureUseVar(Procedure procedure, Variable variable);

  private:
    PKB& pkb;
};
