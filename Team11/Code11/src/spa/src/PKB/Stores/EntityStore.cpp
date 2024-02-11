#include "pkb/Stores/EntityStore.h"

EntityStore::EntityStore() = default;

EntityStore::~EntityStore() = default;

void EntityStore::addVariable(Variable& variable) {
    variableSet.insert(variable);
}

void EntityStore::addProcedure(Procedure& procedure) {
    procedureSet.insert(procedure);
}

void EntityStore::addConstant(Constant& constant) {
    constantSet.insert(constant);
}

EntityStore::ProcedureSet EntityStore::getProcedures() {
    return procedureSet;
}

EntityStore::VariableSet EntityStore::getVariables() {
    return variableSet;
}

EntityStore::ConstantSet EntityStore::getConstants() {
    return constantSet;
}
