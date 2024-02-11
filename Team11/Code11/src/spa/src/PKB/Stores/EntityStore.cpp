#include "PKB/Stores/EntityStore.h"

EntityStore::EntityStore() = default;

void EntityStore::addVariable(const Variable &variable) {
    variableSet.insert(variable);
}

void EntityStore::addProcedure(const Procedure &procedure) {
    procedureSet.insert(procedure);
}

void EntityStore::addConstant(const Constant &constant) {
    constantSet.insert(constant);
}

EntityStore::ProcedureSet EntityStore::getProcedures() const {
    return procedureSet;
}

EntityStore::VariableSet EntityStore::getVariables() const {
    return variableSet;
}

EntityStore::ConstantSet EntityStore::getConstants() const {
    return constantSet;
}
