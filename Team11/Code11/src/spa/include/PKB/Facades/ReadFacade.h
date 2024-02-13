#pragma once

#include "PKB/CommonTypes/Constant.h"
#include "PKB/CommonTypes/Entity.h"
#include "PKB/CommonTypes/Procedure.h"
#include "PKB/CommonTypes/Statement.h"
#include "PKB/CommonTypes/Variable.h"
#include "PKB/PKB.h"
#include <memory>
#include <unordered_set>

class ReadFacade {
  public:
    using StatementNumber = std::string;

    explicit ReadFacade(std::shared_ptr<PKB> pkb);

    // Entity-related Read Operations
    std::unordered_set<std::string> getEntities();

    std::unordered_set<std::string> getProcedures();

    std::unordered_set<std::string> getVariables();

    std::unordered_set<std::string> getConstants();

    // Statement-related Read Operations
    std::unordered_set<std::string> getAllStatements();

    std::unordered_set<std::string> getRawStatements();

    std::unordered_set<std::string> getAssignStatements();

    std::unordered_set<std::string> getIfStatements();

    std::unordered_set<std::string> getWhileStatements();

    std::unordered_set<std::string> getReadStatements();

    std::unordered_set<std::string> getPrintStatements();

    std::unordered_set<std::string> getCallStatements();

    // Modifies-related Read Operations
    std::unordered_set<std::string> getVarsModifiedByStatement(const std::string& s);

    std::unordered_set<std::string> getStatementsThatModifyVar(const std::string& variable);

    bool doesStatementModifyVar(const std::string& statement_number, const std::string& variable);

    std::unordered_set<std::string> getVarsModifiedByProcedure(const std::string& procedure);

    std::unordered_set<std::string> getProceduresThatModifyVar(const std::string& variable);

    bool doesProcedureModifyVar(const std::string& procedure, const std::string& variable);

    // Uses-related Read Operations
    std::unordered_set<std::string> getVarsUsedByStatement(const std::string& s);

    std::unordered_set<std::string> getStatementsThatUseVar(const std::string& variable);

    bool doesStatementUseVar(const std::string& statement_number, const std::string& variable);

    std::unordered_set<std::string> getVarsUsedByProcedure(const std::string& procedure);

    std::unordered_set<std::string> getProceduresThatUseVar(const std::string& variable);

    bool doesProcedureUseVar(const std::string& procedure, const std::string& variable);

    // Follows-related Read Operations
    bool hasFollows(const std::string& stmt1, const std::string& stmt2) const;

    std::unordered_map<std::string, std::string> getAllFollows() const;

    // Gets all statements followed by another statement from Follows relationship
    std::unordered_set<std::string> getAllFollowsKeys() const;

    // Gets all statements following another statement from Follows relationship
    std::unordered_set<std::string> getAllFollowsValues() const;

    std::string getFollowsFollowing(const std::string& s) const;

    std::string getFollowsBy(const std::string& s) const;

    bool hasFollowsStars(const std::string& stmt1, const std::string& stmt2) const;

    // Gets all transitive Follows relationship
    std::unordered_map<std::string, std::unordered_set<std::string>> getAllFollowsStar() const;

    // Gets all statements followed by another statement from Follows relationship
    std::unordered_set<std::string> getAllFollowsStarKeys() const;

    // Gets all statements following another statement from Follows relationship
    std::unordered_set<std::string> getAllFollowsStarValues() const;

    std::unordered_set<std::string> getFollowsStarsFollowing(const std::string& stmt) const;

    std::unordered_set<std::string> getFollowsStarsBy(const std::string& stmt) const;

    // Parent-related Read Operations
    bool hasParent(const std::string& parent, const std::string& child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAllParent() const;

    std::unordered_set<std::string> getAllParentKeys() const;

    std::unordered_set<std::string> getAllParentValues() const;

    std::unordered_set<std::string> getParentChildren(const std::string& parent) const;

    std::string getParent(const std::string& child) const;

    bool hasParentStar(const std::string& parent, const std::string& child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAllParentStar() const;

    std::unordered_set<std::string> getAllParentStarKeys() const;

    std::unordered_set<std::string> getAllParentStarValues() const;

    std::unordered_set<std::string> getParentStarChildren(const std::string& parent) const;

    std::unordered_set<std::string> getStarParent(const std::string& child) const;

    // Next-related Read Operations
    bool hasNext(const std::string& before, const std::string& after) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAllNext() const;

    std::unordered_set<std::string> getAllNextKeys() const;

    std::unordered_set<std::string> getAllNextValues() const;

    std::unordered_set<std::string> getNext(const std::string& before) const;

    std::unordered_set<std::string> getNextPrevious(const std::string& after) const;

    bool hasNextStar(const std::string& before, const std::string& after) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAllNextStar() const;

    std::unordered_set<std::string> getAllNextStarKeys() const;

    std::unordered_set<std::string> getAllNextStarValues() const;

    std::unordered_set<std::string> getNextStar(const std::string& before) const;

    std::unordered_set<std::string> getNextStarPrevious(const std::string& after) const;

    // Calls-related Read Operations
    bool hasCalls(const std::string& caller, const std::string& callee) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAllCalls() const;

    std::unordered_set<std::string> getAllCallsValues() const;

    std::unordered_set<std::string> getAllCallsKeys() const;

    std::unordered_set<std::string> getCallees(const std::string& caller) const;

    std::unordered_set<std::string> getCallers(const std::string& callee) const;

    bool hasCallsStar(const std::string& caller, const std::string& callee) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAllCallsStar() const;

    std::unordered_set<std::string> getAllCallsStarValues() const;

    std::unordered_set<std::string> getAllCallsStarKeys() const;

    std::unordered_set<std::string> getCallsStarCallees(const std::string& caller) const;

    std::unordered_set<std::string> getCallsStarCallers(const std::string& callee) const;

  private:
    std::shared_ptr<PKB> pkb;
};