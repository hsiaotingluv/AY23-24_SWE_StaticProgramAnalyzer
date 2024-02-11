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
    std::unordered_set<Statement> getStatements();

    std::unordered_set<Statement> getAssignStatements();

    std::unordered_set<Statement> getIfStatements();

    std::unordered_set<Statement> getWhileStatements();

    std::unordered_set<Statement> getReadStatements();

    std::unordered_set<Statement> getPrintStatements();

    std::unordered_set<Statement> getCallStatements();

    // Modifies-related Read Operations
    std::unordered_set<std::string> getVarsModifiedByStatement(std::string s);

    std::unordered_set<std::string> getStatementsThatModifyVar(std::string variable);

    bool doesStatementModifyVar(const std::string &statement_number, std::string variable);

    std::unordered_set<std::string> getVarsModifiedByProcedure(std::string procedure);

    std::unordered_set<std::string> getProceduresThatModifyVar(std::string variable);

    bool doesProcedureModifyVar(std::string procedure, std::string variable);

    // Uses-related Read Operations
    std::unordered_set<std::string> getVarsUsedByStatement(std::string s);

    std::unordered_set<std::string> getStatementsThatUseVar(std::string variable);

    bool doesStatementUseVar(const std::string &statement_number, std::string variable);

    std::unordered_set<std::string> getVarsUsedByProcedure(std::string procedure);

    std::unordered_set<std::string> getProceduresThatUseVar(std::string variable);

    bool doesProcedureUseVar(std::string procedure, std::string variable);

    // Follows-related Read Operations
    bool hasFollows(const std::string &stmt1, const std::string &stmt2) const;

    std::unordered_map<std::string, std::string> getAllFollows() const;

    // Gets all statements followed by another statement from Follows relationship
    std::unordered_set<std::string> getAllFollowsKeys() const;

    // Gets all statements following another statement from Follows relationship
    std::unordered_set<std::string> getAllFollowsValues() const;

    std::string getFollowsFollowing(const std::string &s) const;

    std::string getFollowsBy(const std::string &s) const;

    bool hasFollowsStars(const std::string &stmt1, const std::string &stmt2) const;

    // Gets all transitive Follows relationship
    std::unordered_map<std::string, std::unordered_set<std::string>> getAllFollowsStar() const;

    // Gets all statements followed by another statement from Follows relationship
    std::unordered_set<std::string> getAllFollowsStarKeys() const;

    // Gets all statements following another statement from Follows relationship
    std::unordered_set<std::string> getAllFollowsStarValues() const;

    std::unordered_set<std::string> getFollowsStarsFollowing(const std::string &stmt) const;

    std::unordered_set<std::string> getFollowsStarsBy(const std::string &stmt) const;

    // Parent-related Read Operations
    bool hasParent(const std::string &parent, const std::string &child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAllParent() const;

    std::unordered_set<std::string> getAllParentKeys() const;

    std::unordered_set<std::string> getAllParentValues() const;

    std::unordered_set<std::string> getParentChildren(const std::string &parent) const;

    std::string getParent(const std::string &child) const;

    bool hasParentStar(const std::string &parent, const std::string &child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> getAllParentStar() const;

    std::unordered_set<std::string> getAllParentStarKeys() const;

    std::unordered_set<std::string> getAllParentStarValues() const;

    std::unordered_set<std::string> getParentStarChildren(const std::string &parent) const;

    std::unordered_set<std::string> getStarParent(const std::string &child) const;

private:
    std::shared_ptr<PKB> pkb;
};
