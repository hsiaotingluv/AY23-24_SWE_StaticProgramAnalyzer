#include "PKB/Facades/ReadFacade.h"

#include <utility>

ReadFacade::ReadFacade(std::shared_ptr<PKB> pkb) : pkb(std::move(pkb)) {
}

std::unordered_set<std::string> ReadFacade::getEntities() {
    std::unordered_set<std::string> entities;

    auto procedures = this->pkb->entity_store->getProcedures();
    auto vars = this->pkb->entity_store->getVariables();
    auto consts = this->pkb->entity_store->getConstants();

    for (const Procedure& p : procedures) {
        entities.insert(p.getName());
    }
    for (const Variable& v : vars) {
        entities.insert(v.getName());
    }
    for (const Constant& c : consts) {
        entities.insert(c.getName());
    }

    return entities;
}

std::unordered_set<std::string> ReadFacade::getProcedures() {
    auto procedures = this->pkb->entity_store->getProcedures();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::getVariables() {
    auto vars = this->pkb->entity_store->getVariables();

    std::unordered_set<std::string> temp;
    for (const Variable& v : vars) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::getConstants() {
    auto consts = this->pkb->entity_store->getConstants();

    std::unordered_set<std::string> temp;
    for (const Constant& c : consts) {
        temp.insert(c.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::getAllStatements() {
    return this->pkb->statement_store->getStatements();
}

std::unordered_set<std::string> ReadFacade::getRawStatements() {
    return this->pkb->statement_store->getStatementsOfType(StatementType::Raw);
}

std::unordered_set<std::string> ReadFacade::getAssignStatements() {
    return this->pkb->statement_store->getStatementsOfType(StatementType::Assign);
}

std::unordered_set<std::string> ReadFacade::getIfStatements() {
    return this->pkb->statement_store->getStatementsOfType(StatementType::If);
}

std::unordered_set<std::string> ReadFacade::getWhileStatements() {
    return this->pkb->statement_store->getStatementsOfType(StatementType::While);
}

std::unordered_set<std::string> ReadFacade::getReadStatements() {
    return this->pkb->statement_store->getStatementsOfType(StatementType::Read);
}

std::unordered_set<std::string> ReadFacade::getPrintStatements() {
    return this->pkb->statement_store->getStatementsOfType(StatementType::Print);
}

std::unordered_set<std::string> ReadFacade::getCallStatements() {
    return this->pkb->statement_store->getStatementsOfType(StatementType::Call);
}

std::unordered_set<std::string> ReadFacade::getVarsModifiedByStatement(const std::string& s) {
    auto variables = this->pkb->modifies_store->getVarsModifiedByStatement(s);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::getStatementsThatModifyVar(const std::string& variable) {
    auto v = Variable(variable);

    return this->pkb->modifies_store->getStatementsThatModifyVar(v);
}

bool ReadFacade::doesStatementModifyVar(const std::string& statement, const std::string& variable) {
    auto v = Variable(variable);

    return this->pkb->modifies_store->doesStatementModifyVar(statement, v);
}

std::unordered_set<std::string> ReadFacade::getVarsModifiedByProcedure(const std::string& procedure) {
    auto p = Procedure(procedure);

    auto variables = this->pkb->modifies_store->getVarsModifiedByProcedure(p);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::getProceduresThatModifyVar(const std::string& variable) {
    auto v = Variable(variable);

    auto procedures = this->pkb->modifies_store->getProceduresThatModifyVar(v);

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool ReadFacade::doesProcedureModifyVar(const std::string& procedure, const std::string& variable) {
    auto v = Variable(variable);
    auto p = Procedure(procedure);

    return this->pkb->modifies_store->doesProcedureModifyVar(p, v);
}

std::unordered_set<std::string> ReadFacade::getVarsUsedByStatement(const std::string& s) {
    auto variables = this->pkb->uses_store->getVarsUsedByStatement(s);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::getStatementsThatUseVar(const std::string& variable) {
    auto v = Variable(variable);

    return this->pkb->uses_store->getStatementsThatUseVar(v);
}

bool ReadFacade::doesStatementUseVar(const std::string& statement, const std::string& variable) {
    auto v = Variable(variable);

    return this->pkb->uses_store->doesStatementUseVar(statement, v);
}

std::unordered_set<std::string> ReadFacade::getVarsUsedByProcedure(const std::string& procedure) {
    auto p = Procedure(procedure);

    auto variables = this->pkb->uses_store->getVarsUsedByProcedure(p);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> ReadFacade::getProceduresThatUseVar(const std::string& variable) {
    auto v = Variable(variable);

    auto procedures = this->pkb->uses_store->getProceduresThatUseVar(v);

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool ReadFacade::doesProcedureUseVar(const std::string& procedure, const std::string& variable) {
    auto v = Variable(variable);
    auto p = Procedure(procedure);

    return this->pkb->uses_store->doesProcedureUseVar(p, v);
}

std::unordered_map<std::string, std::string> ReadFacade::getAllFollows() const {
    return this->pkb->follows_store->getAllFollows();
}

std::unordered_set<std::string> ReadFacade::getAllFollowsKeys() const {
    return this->pkb->follows_store->getAllFollowsKeys();
}

std::unordered_set<std::string> ReadFacade::getAllFollowsValues() const {
    return this->pkb->follows_store->getAllFollowsValues();
}

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
}

std::unordered_set<std::string> ReadFacade::getAllFollowsStarKeys() const {
    return this->pkb->follows_store->getAllFollowsStarKeys();
}

std::unordered_set<std::string> ReadFacade::getAllFollowsStarValues() const {
    return this->pkb->follows_store->getAllFollowsStarValues();
}

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
}

std::unordered_set<std::string> ReadFacade::getAllParentKeys() const {
    return this->pkb->parent_store->getAllParentKeys();
}

std::unordered_set<std::string> ReadFacade::getAllParentValues() const {
    return this->pkb->parent_store->getAllParentValues();
}

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
}

std::unordered_set<std::string> ReadFacade::getAllParentStarKeys() const {
    return this->pkb->parent_store->getAllParentStarKeys();
}

std::unordered_set<std::string> ReadFacade::getAllParentStarValues() const {
    return this->pkb->parent_store->getAllParentStarValues();
}

std::unordered_set<std::string> ReadFacade::getParentStarChildren(const std::string& parent) const {
    return this->pkb->parent_store->getParentStarChildren(parent);
}

std::unordered_set<std::string> ReadFacade::getStarParent(const std::string& child) const {
    return this->pkb->parent_store->getStarParent(child);
}

bool ReadFacade::hasNext(const std::string& before, const std::string& after) const {
    return this->pkb->next_store->hasNext(before, after);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::getAllNext() const {
    return this->pkb->next_store->getAllNext();
}

std::unordered_set<std::string> ReadFacade::getAllNextKeys() const {
    return this->pkb->next_store->getAllNextKeys();
}

std::unordered_set<std::string> ReadFacade::getAllNextValues() const {
    return this->pkb->next_store->getAllNextValues();
}

std::unordered_set<std::string> ReadFacade::getNext(const std::string& before) const {
    return this->pkb->next_store->getNext(before);
}

std::unordered_set<std::string> ReadFacade::getNextPrevious(const std::string& after) const {
    return this->pkb->next_store->getNextPrevious(after);
}

bool ReadFacade::hasNextStar(const std::string& before, const std::string& after) const {
    return this->pkb->next_store->hasNextStar(before, after);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::getAllNextStar() const {
    return this->pkb->next_store->getAllNextStar();
}

std::unordered_set<std::string> ReadFacade::getAllNextStarKeys() const {
    return this->pkb->next_store->getAllNextStarKeys();
}

std::unordered_set<std::string> ReadFacade::getAllNextStarValues() const {
    return this->pkb->next_store->getAllNextStarValues();
}

std::unordered_set<std::string> ReadFacade::getNextStar(const StatementNumber& before) const {
    return this->pkb->next_store->getNextStar(before);
}

std::unordered_set<std::string> ReadFacade::getNextStarPrevious(const StatementNumber& after) const {
    return this->pkb->next_store->getNextStarPrevious(after);
}

bool ReadFacade::hasCalls(const std::string& caller, const std::string& callee) const {
    return this->pkb->calls_store->hasCalls(caller, callee);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::getAllCalls() const {
    return this->pkb->calls_store->getAllCalls();
}

std::unordered_set<std::string> ReadFacade::getAllCallsValues() const {
    return this->pkb->calls_store->getAllCallsValues();
}

std::unordered_set<std::string> ReadFacade::getAllCallsKeys() const {
    return this->pkb->calls_store->getAllCallsKeys();
}

std::unordered_set<std::string> ReadFacade::getCallees(const std::string& caller) const {
    return this->pkb->calls_store->getCallees(caller);
}

std::unordered_set<std::string> ReadFacade::getCallers(const std::string& callee) const {
    return this->pkb->calls_store->getCallers(callee);
}

bool ReadFacade::hasCallsStar(const std::string& caller, const std::string& callee) const {
    return this->pkb->calls_store->hasCallsStar(caller, callee);
}

std::unordered_map<std::string, std::unordered_set<std::string>> ReadFacade::getAllCallsStar() const {
    return this->pkb->calls_store->getAllCallsStar();
}

std::unordered_set<std::string> ReadFacade::getAllCallsStarValues() const {
    return this->pkb->calls_store->getAllCallsStarValues();
}

std::unordered_set<std::string> ReadFacade::getAllCallsStarKeys() const {
    return this->pkb->calls_store->getAllCallsStarKeys();
}

std::unordered_set<std::string> ReadFacade::getCallsStarCallees(const std::string& caller) const {
    return this->pkb->calls_store->getCallsStarCallees(caller);
}

std::unordered_set<std::string> ReadFacade::getCallsStarCallers(const std::string& callee) const {
    return this->pkb->calls_store->getCallsStarCallers(callee);
}