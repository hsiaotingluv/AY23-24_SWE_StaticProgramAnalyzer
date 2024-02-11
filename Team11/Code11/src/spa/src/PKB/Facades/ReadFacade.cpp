#include "PKB/Facades/ReadFacade.h"

#include <utility>

ReadFacade::ReadFacade(std::shared_ptr<PKB> pkb) : pkb(std::move(pkb)) {
}

std::unordered_set<std::string> ReadFacade::getEntities() {
    std::unordered_set<std::string> entities;

    auto procedures = this->pkb->entity_store->getProcedures();
    auto vars = this->pkb->entity_store->getVariables();
    auto consts = this->pkb->entity_store->getConstants();

    for (const Procedure& p : procedures)
        entities.insert(p.getName());
    for (const Variable& v : vars)
        entities.insert(v.getName());
    for (const Constant& c : consts)
        entities.insert(c.getName());

    return entities;
}

std::unordered_set<std::string> ReadFacade::getProcedures() {
    auto procedures = this->pkb->entity_store->getProcedures();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures)
        temp.insert(p.getName());

    return temp;
}

std::unordered_set<std::string> ReadFacade::getVariables() {
    auto vars = this->pkb->entity_store->getVariables();

    std::unordered_set<std::string> temp;
    for (const Variable& v : vars)
        temp.insert(v.getName());

    return temp;
}

std::unordered_set<std::string> ReadFacade::getConstants() {
    auto consts = this->pkb->entity_store->getConstants();

    std::unordered_set<std::string> temp;
    for (const Constant& c : consts)
        temp.insert(c.getName());

    return temp;
}

std::unordered_set<Statement> ReadFacade::getStatements() {
    // TODO: Replace with actual implementation for getStatements
    return std::unordered_set<Statement>{Statement("Statement1"), Statement("Statement2"), Statement("Statement3")};
}

std::unordered_set<Statement> ReadFacade::getAssignStatements() {
    // TODO: Replace with actual implementation for getAssignStatements
    return std::unordered_set<Statement>{Statement("AssignStatement1"), Statement("AssignStatement2"),
                                         Statement("AssignStatement3")};
}

std::unordered_set<Statement> ReadFacade::getIfStatements() {
    // TODO: Replace with actual implementation for getIfStatements
    return std::unordered_set<Statement>{Statement("IfStatement1"), Statement("IfStatement2"),
                                         Statement("IfStatement3")};
}

std::unordered_set<Statement> ReadFacade::getWhileStatements() {
    // TODO: Replace with actual implementation for getWhileStatements
    return std::unordered_set<Statement>{Statement("WhileStatement1"), Statement("WhileStatement2"),
                                         Statement("WhileStatement3")};
}

std::unordered_set<Statement> ReadFacade::getReadStatements() {
    // TODO: Replace with actual implementation for getReadStatements
    return std::unordered_set<Statement>{Statement("ReadStatement1"), Statement("ReadStatement2"),
                                         Statement("ReadStatement3")};
}

std::unordered_set<Statement> ReadFacade::getPrintStatements() {
    // TODO: Replace with actual implementation for getPrintStatements
    return std::unordered_set<Statement>{Statement("PrintStatement1"), Statement("PrintStatement2"),
                                         Statement("PrintStatement3")};
}

std::unordered_set<Statement> ReadFacade::getCallStatements() {
    // TODO: Replace with actual implementation for getCallStatements
    return std::unordered_set<Statement>{Statement("CallStatement1"), Statement("CallStatement2"),
                                         Statement("CallStatement3")};
}

std::unordered_set<std::string> ReadFacade::getVarsModifiedByStatement(std::string s) {
    auto variables = this->pkb->modifies_store->getVarsModifiedByStatement(s);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables)
        temp.insert(v.getName());

    return temp;
}

std::unordered_set<std::string> ReadFacade::getStatementsThatModifyVar(std::string variable) {
    auto v = Variable(std::move(variable));

    return this->pkb->modifies_store->getStatementsThatModifyVar(v);
}

bool ReadFacade::doesStatementModifyVar(const std::string& statement, std::string variable) {
    auto v = Variable(std::move(variable));

    return this->pkb->modifies_store->doesStatementModifyVar(statement, v);
}

std::unordered_set<std::string> ReadFacade::getVarsModifiedByProcedure(std::string procedure) {
    auto p = Procedure(std::move(procedure));

    auto variables = this->pkb->modifies_store->getVarsModifiedByProcedure(p);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables)
        temp.insert(v.getName());

    return temp;
}

std::unordered_set<std::string> ReadFacade::getProceduresThatModifyVar(std::string variable) {
    auto v = Variable(std::move(variable));

    auto procedures = this->pkb->modifies_store->getProceduresThatModifyVar(v);

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures)
        temp.insert(p.getName());

    return temp;
}

bool ReadFacade::doesProcedureModifyVar(std::string procedure, std::string variable) {
    auto v = Variable(std::move(variable));
    auto p = Procedure(std::move(procedure));

    return this->pkb->modifies_store->doesProcedureModifyVar(p, v);
}

std::unordered_set<std::string> ReadFacade::getVarsUsedByStatement(std::string s) {
    auto variables = this->pkb->uses_store->getVarsUsedByStatement(s);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables)
        temp.insert(v.getName());

    return temp;
}

std::unordered_set<std::string> ReadFacade::getStatementsThatUseVar(std::string variable) {
    auto v = Variable(std::move(variable));

    return this->pkb->uses_store->getStatementsThatUseVar(v);
}

bool ReadFacade::doesStatementUseVar(const std::string& statement, std::string variable) {
    auto v = Variable(std::move(variable));

    return this->pkb->uses_store->doesStatementUseVar(statement, v);
}

std::unordered_set<std::string> ReadFacade::getVarsUsedByProcedure(std::string procedure) {
    auto p = Procedure(std::move(procedure));

    auto variables = this->pkb->uses_store->getVarsUsedByProcedure(p);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables)
        temp.insert(v.getName());

    return temp;
}

std::unordered_set<std::string> ReadFacade::getProceduresThatUseVar(std::string variable) {
    auto v = Variable(std::move(variable));

    auto procedures = this->pkb->uses_store->getProceduresThatUseVar(v);

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures)
        temp.insert(p.getName());

    return temp;
}

bool ReadFacade::doesProcedureUseVar(std::string procedure, std::string variable) {
    auto v = Variable(std::move(variable));
    auto p = Procedure(std::move(procedure));

    return this->pkb->uses_store->doesProcedureUseVar(p, v);
}

std::unordered_map<std::string, std::string> ReadFacade::getAllFollows() const {
    return this->pkb->follows_store->getAllFollows();
};

std::unordered_set<std::string> ReadFacade::getAllFollowsKeys() const {
    return this->pkb->follows_store->getAllFollowsKeys();
};

std::unordered_set<std::string> ReadFacade::getAllFollowsValues() const {
    return this->pkb->follows_store->getAllFollowsValues();
};

bool ReadFacade::hasFollows(const std::string& stmt1, const std::string& stmt2) const {
    return this->pkb->follows_store->hasFollows(stmt1, stmt2);
}

std::string ReadFacade::getFollowsFollowing(const std::string& s) const {
    return this->pkb->follows_store->getFollowsFollowing(s);
}

std::string ReadFacade::getFollowsBy(const std::string& s) const {
    return this->pkb->follows_store->getFollowsBy(s);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::getAllFollowsStar() const {
    return this->pkb->follows_store->getAllFollowsStar();
};

std::unordered_set<std::string> ReadFacade::getAllFollowsStarKeys() const {
    return this->pkb->follows_store->getAllFollowsStarKeys();
};

std::unordered_set<std::string> ReadFacade::getAllFollowsStarValues() const {
    return this->pkb->follows_store->getAllFollowsStarValues();
};

bool ReadFacade::hasFollowsStars(const std::string& stmt1, const std::string& stmt2) const {
    return this->pkb->follows_store->hasFollowsStars(stmt1, stmt2);
}

std::unordered_set<std::string> ReadFacade::getFollowsStarsFollowing(const std::string& stmt) const {
    return this->pkb->follows_store->getFollowsStarsFollowing(stmt);
}

std::unordered_set<std::string> ReadFacade::getFollowsStarsBy(const std::string& stmt) const {
    return this->pkb->follows_store->getFollowsStarsBy(stmt);
}

bool ReadFacade::hasParent(const std::string& parent, const std::string& child) const {
    return this->pkb->parent_store->hasParent(parent, child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::getAllParent() const {
    return this->pkb->parent_store->getAllParent();
};

std::unordered_set<std::string> ReadFacade::getAllParentKeys() const {
    return this->pkb->parent_store->getAllParentKeys();
};

std::unordered_set<std::string> ReadFacade::getAllParentValues() const {
    return this->pkb->parent_store->getAllParentValues();
};

std::unordered_set<std::string> ReadFacade::getParentChildren(const std::string& parent) const {
    return this->pkb->parent_store->getParentChildren(parent);
}

std::string ReadFacade::getParent(const std::string& child) const {
    return this->pkb->parent_store->getParent(child);
}

bool ReadFacade::hasParentStar(const std::string& parent, const std::string& child) const {
    return this->pkb->parent_store->hasParentStar(parent, child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::getAllParentStar() const {
    return this->pkb->parent_store->getAllParentStar();
};

std::unordered_set<std::string> ReadFacade::getAllParentStarKeys() const {
    return this->pkb->parent_store->getAllParentStarKeys();
};

std::unordered_set<std::string> ReadFacade::getAllParentStarValues() const {
    return this->pkb->parent_store->getAllParentStarValues();
};

std::unordered_set<std::string> ReadFacade::getParentStarChildren(const std::string& parent) const {
    return this->pkb->parent_store->getParentStarChildren(parent);
}

std::unordered_set<std::string> ReadFacade::getStarParent(const std::string& child) const {
    return this->pkb->parent_store->getStarParent(child);
}
