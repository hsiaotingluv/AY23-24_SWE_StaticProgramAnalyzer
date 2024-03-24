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

bool EntityStore::has_procedure(const std::string& procedure) const {
    auto p = Procedure(procedure);
    return procedureSet.find(p) != procedureSet.end();
}

bool EntityStore::has_variable(const std::string& variable) const {
    auto v = Variable(variable);
    return variableSet.find(v) != variableSet.end();
}

bool EntityStore::has_constant(const std::string& constant) const {
    auto c = Constant(constant);
    return constantSet.find(c) != constantSet.end();
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
