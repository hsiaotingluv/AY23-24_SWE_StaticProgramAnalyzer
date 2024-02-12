#include "PKB/Facades/WriteFacade.h"

#include <utility>

WriteFacade::WriteFacade(std::shared_ptr<PKB> pkb) : pkb(std::move(pkb)) {
}

void WriteFacade::addProcedure(std::string procedure) {
    Procedure p = Procedure(std::move(procedure));

    this->pkb->entity_store->addProcedure(p);
}

void WriteFacade::addVariable(std::string variable) {
    Variable v = Variable(std::move(variable));

    this->pkb->entity_store->addVariable(v);
}

void WriteFacade::addConstant(std::string constant) {
    Constant c = Constant(std::move(constant));

    this->pkb->entity_store->addConstant(c);
}

void WriteFacade::addStatement(Statement& statement) {
    // TODO: Replace with actual implementation for addStatement
}

void WriteFacade::addStatementModifiesVar(const std::string& statementNumber, std::string variable) {
    auto v = Variable(std::move(variable));
    this->pkb->modifies_store->addStatementModifiesVar(statementNumber, v);
}

void WriteFacade::addProcedureModifiesvar(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    this->pkb->modifies_store->addProcedureModifiesVar(p, v);
}

void WriteFacade::addStatementUsesVar(const std::string& statementNumber, std::string variable) {
    auto v = Variable(std::move(variable));
    this->pkb->uses_store->addStatementUsesVar(statementNumber, v);
}

void WriteFacade::addProcedureUsesvar(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    this->pkb->uses_store->addProcedureUsesVar(p, v);
}

void WriteFacade::addFollows(const std::string& stmt1, const std::string& stmt2) {
    this->pkb->follows_store->addFollows(stmt1, stmt2);
}

void WriteFacade::addFollowStars(const std::string& stmt1, const std::string& stmt2) {
    this->pkb->follows_store->addFollowsStars(stmt1, stmt2);
}

void WriteFacade::addParent(const std::string& parent, const std::string& child) {
    this->pkb->parent_store->addParent(parent, child);
}

void WriteFacade::addParentStar(const std::string& parent, const std::string& child) {
    this->pkb->parent_store->addParentStar(parent, child);
}
