#include "pkb/stores/calls_store.h"

CallsStore::CallsStore() = default;

void CallsStore::add_calls(const ProcedureName& caller, const ProcedureName& callee) {
    calls_store[caller].insert(callee);
    reversed_calls_store[callee].insert(caller);

    calls_star_store[caller].insert(callee);
    reversed_calls_star_store[callee].insert(caller);
}

bool CallsStore::has_calls(const ProcedureName& caller, const ProcedureName& callee) const {
    auto it = calls_store.find(caller);
    return it != calls_store.end() && it->second.find(callee) != it->second.end();
}

CallsStore::ProcedureToProcedureSetMap CallsStore::get_all_calls() const {
    return calls_store;
}

CallsStore::ProcedureSet CallsStore::get_all_calls_values() const {
    ProcedureSet allCallees;
    for (const auto& pair : calls_store) {
        allCallees.insert(pair.second.begin(), pair.second.end());
    }
    return allCallees;
}

CallsStore::ProcedureSet CallsStore::get_all_calls_keys() const {
    ProcedureSet allCallers;
    for (const auto& pair : calls_store) {
        allCallers.insert(pair.first);
    }
    return allCallers;
}

CallsStore::ProcedureSet CallsStore::get_callees(const ProcedureName& caller) const {
    auto it = calls_store.find(caller);
    if (it != calls_store.end()) {
        return it->second;
    }
    return {};
}

CallsStore::ProcedureSet CallsStore::get_callers(const ProcedureName& callee) const {
    auto it = reversed_calls_store.find(callee);
    if (it != reversed_calls_store.end()) {
        return it->second;
    }
    return {};
}

void CallsStore::add_calls_star(const ProcedureName& caller, const ProcedureName& callee) {
    calls_star_store[caller].insert(callee);
    reversed_calls_star_store[callee].insert(caller);
}

bool CallsStore::has_calls_star(const ProcedureName& caller, const ProcedureName& callee) const {
    auto it = calls_star_store.find(caller);
    return it != calls_star_store.end() && it->second.find(callee) != it->second.end();
}

CallsStore::ProcedureToProcedureSetMap CallsStore::get_all_calls_star() const {
    return calls_star_store;
}

CallsStore::ProcedureSet CallsStore::get_all_calls_star_values() const {
    ProcedureSet allCallees;
    for (const auto& pair : calls_star_store) {
        allCallees.insert(pair.second.begin(), pair.second.end());
    }
    return allCallees;
}

CallsStore::ProcedureSet CallsStore::get_all_calls_star_keys() const {
    ProcedureSet allCallers;
    for (const auto& pair : calls_star_store) {
        allCallers.insert(pair.first);
    }
    return allCallers;
}

CallsStore::ProcedureSet CallsStore::get_calls_star_callees(const ProcedureName& caller) const {
    auto it = calls_star_store.find(caller);
    if (it != calls_star_store.end()) {
        return it->second;
    }
    return {};
}

CallsStore::ProcedureSet CallsStore::get_calls_star_callers(const ProcedureName& callee) const {
    auto it = reversed_calls_star_store.find(callee);
    if (it != reversed_calls_star_store.end()) {
        return it->second;
    }
    return {};
}
