#pragma once

#include "PKB/CommonTypes/Constant.h"
#include "PKB/CommonTypes/Entity.h"
#include "PKB/CommonTypes/Procedure.h"
#include "PKB/CommonTypes/Statement.h"
#include "PKB/CommonTypes/Variable.h"
#include "PKB/PKB.h"
#include "common/statement_type.hpp"
#include <unordered_set>

class WriteFacade {
  public:
    typedef std::unordered_set<std::string> StringSet;

    explicit WriteFacade(std::shared_ptr<PKB> pkb);

    void addProcedure(std::string procedure);

    void addVariable(std::string variable);

    void addConstant(std::string constant);

    void addStatement(const std::string& statementNumber, StatementType type);

    void addStatementModifiesVar(const std::string& statementNumber, std::string variable);

    void addProcedureModifiesvar(std::string procedure, std::string variable);

    void addStatementUsesVar(const std::string& statementNumber, std::string variable);

    void addProcedureUsesvar(std::string procedure, std::string variable);

    void addFollows(const std::string& stmt1, const std::string& stmt2);

    void addFollowStars(const std::string& stmt1, const std::string& stmt2);

    void addParent(const std::string& parent, const std::string& child);

    void addParentStar(const std::string& parent, const std::string& child);

    void addNext(const std::string& before, const std::string& after);

    void addNextStar(const std::string& before, const std::string& after);

    void addCalls(const std::string& caller, const std::string& callee);

    void addCallsStar(const std::string& caller, const std::string& callee);

  private:
    std::shared_ptr<PKB> pkb;
};
