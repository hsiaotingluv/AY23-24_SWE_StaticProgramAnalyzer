#include "pkb/stores/entity_store.h"

EntityStore::EntityStore() = default;

void EntityStore::add_variable(const Variable& variable) {
    variableSet.insert(variable);
}

void EntityStore::add_procedure(const Procedure& procedure) {
    procedureSet.insert(procedure);
}

void EntityStore::add_constant(const Constant& constant) {
    constantSet.insert(constant);
}

EntityStore::ProcedureSet EntityStore::get_procedures() const {
    return procedureSet;
}

EntityStore::VariableSet EntityStore::get_variables() const {
    return variableSet;
}

EntityStore::ConstantSet EntityStore::get_constants() const {
    return constantSet;
}
