#include "pkb/Facades/ReadFacade.h"

ReadFacade::ReadFacade(PKB &pkb) : pkb(pkb) {}

ReadFacade::~ReadFacade() = default;

ReadFacade::StringSet ReadFacade::getEntities() {
    auto allEntities = this->getConstants();

    allEntities.merge(this->getVariables());
    allEntities.merge(this->getProcedures());

    return allEntities;
}

ReadFacade::StringSet ReadFacade::getProcedures() { return this->pkb.entity_store->getProcedures(); }

ReadFacade::StringSet ReadFacade::getVariables() { return this->pkb.entity_store->getVariables(); }

ReadFacade::StringSet ReadFacade::getConstants() { return this->pkb.entity_store->getConstants(); }

ReadFacade::StringSet ReadFacade::getStatements() {
    // TODO: Replace with actual implementation for getStatements
    return StringSet{"Statement1", "Statement2", "Statement3"};
}

ReadFacade::StringSet ReadFacade::getAssignStatements() {
    // TODO: Replace with actual implementation for getAssignStatements
    return StringSet{"AssignStatement1", "AssignStatement2", "AssignStatement3"};
}

ReadFacade::StringSet ReadFacade::getIfStatements() {
    // TODO: Replace with actual implementation for getIfStatements
    return StringSet{"IfStatement1", "IfStatement2", "IfStatement3"};
}

ReadFacade::StringSet ReadFacade::getWhileStatements() {
    // TODO: Replace with actual implementation for getWhileStatements
    return StringSet{"WhileStatement1", "WhileStatement2", "WhileStatement3"};
}

ReadFacade::StringSet ReadFacade::getReadStatements() {
    // TODO: Replace with actual implementation for getReadStatements
    return StringSet{"ReadStatement1", "ReadStatement2", "ReadStatement3"};
}

ReadFacade::StringSet ReadFacade::getPrintStatements() {
    // TODO: Replace with actual implementation for getPrintStatements
    return StringSet{"PrintStatement1", "PrintStatement2", "PrintStatement3"};
}

ReadFacade::StringSet ReadFacade::getCallStatements() {
    // TODO: Replace with actual implementation for getCallStatements
    return StringSet{"CallStatement1", "CallStatement2", "CallStatement3"};
}

ReadFacade::StringSet ReadFacade::getVarsModifiedByStatement(StatementNumber statement_number) {
    // TODO: Replace with actual implementation for getVarsModifiedByStatement
    return StringSet{"ModifiedVar1", "ModifiedVar2", "ModifiedVar3"};
}

ReadFacade::StringSet ReadFacade::getStatementsThatModifyVar(Variable variable) {
    // TODO: Replace with actual implementation for getStatementsThatModifyVar
    return StringSet{"Statement1", "Statement2", "Statement3"};
}

bool ReadFacade::doesStatementModifyVar(StatementNumber statement_number, Variable variable) {
    // TODO: Replace with actual implementation for doesStatementModifyVar
    return true;
}

ReadFacade::StringSet ReadFacade::getVarsModifiedByProcedure(Procedure procedure) {
    // TODO: Replace with actual implementation for getVarsModifiedByProcedure
    return StringSet{"ModifiedVar1", "ModifiedVar2", "ModifiedVar3"};
}

ReadFacade::StringSet ReadFacade::getProceduresThatModifyVar(Variable variable) {
    // TODO: Replace with actual implementation for getProceduresThatModifyVar
    return StringSet{"Procedure1", "Procedure2", "Procedure3"};
}

bool ReadFacade::doesProcedureModifyVar(Procedure procedure, Variable variable) {
    // TODO: Replace with actual implementation for doesProcedureModifyVar
    return true;
}

ReadFacade::StringSet ReadFacade::getVarsUsedByStatement(StatementNumber statement_number) {
    // TODO: Replace with actual implementation for getVarsUsedByStatement
    return StringSet{"UsedVar1", "UsedVar2", "UsedVar3"};
}

ReadFacade::StringSet ReadFacade::getStatementsThatUsesVar(Variable variable) {
    // TODO: Replace with actual implementation for getStatementsThatUsesVar
    return StringSet{"Statement1", "Statement2", "Statement3"};
}

bool ReadFacade::doesStatementUseVar(StatementNumber statement_number, Variable variable) {
    // TODO: Replace with actual implementation for doesStatementUseVar
    return true;
}

ReadFacade::StringSet ReadFacade::getVarsUsedByProcedure(Procedure procedure) {
    // TODO: Replace with actual implementation for getVarsUsedByProcedure
    return StringSet{"UsedVar1", "UsedVar2", "UsedVar3"};
}

ReadFacade::StringSet ReadFacade::getProceduresThatUseVar(Variable variable) {
    // TODO: Replace with actual implementation for getProceduresThatUseVar
    return StringSet{"Procedure1", "Procedure2", "Procedure3"};
}

bool ReadFacade::doesProcedureUseVar(Procedure procedure, Variable variable) {
    // TODO: Replace with actual implementation for doesProcedureUseVar
    return true;
}
