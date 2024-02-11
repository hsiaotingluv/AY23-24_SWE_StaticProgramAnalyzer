#include "pkb/Facades/WriteFacade.h"

WriteFacade::WriteFacade(PKB& pkb) : pkb(pkb) {
}

WriteFacade::~WriteFacade() = default;

void WriteFacade::addProcedure(Procedure& procedure) {
    this->pkb.entity_store->addProcedure(procedure);
}

void WriteFacade::addVariable(Variable& variable) {
    this->pkb.entity_store->addVariable(variable);
}

void WriteFacade::addConstant(Constant& constant) {
    this->pkb.entity_store->addConstant(constant);
}

void WriteFacade::addStatement(StatementNumber statementNumber, StatementType statementType) {
    // TODO: Replace with actual implementation for addStatement
}

void WriteFacade::addStatementModifiesVar(StatementNumber statementNumber, Variable variable) {
    // TODO: Replace with actual implementation for addStatementModifiesVar
}

void WriteFacade::addProcedureModifiesvar(Procedure procedure, Variable variable) {
    // TODO: Replace with actual implementation for addProcedureModifiesVar
}
