#include "PKB/Stores/CallsStore.h"

CallsStore::CallsStore() = default;

void CallsStore::addCalls(const ProcedureName& caller, const ProcedureName& callee) {
    callsStore[caller].insert(callee);
    reversedCallsStore[callee].insert(caller);

    callsStarStore[caller].insert(callee);
    reversedCallsStarStore[callee].insert(caller);
}

bool CallsStore::hasCalls(const ProcedureName& caller, const ProcedureName& callee) const {
    auto it = callsStore.find(caller);
    return it != callsStore.end() && it->second.find(callee) != it->second.end();
}

CallsStore::ProcedureSet CallsStore::getAllCallees() const {
    ProcedureSet allCallees;
    for (const auto& pair : callsStore) {
        allCallees.insert(pair.second.begin(), pair.second.end());
    }
    return allCallees;
}

CallsStore::ProcedureSet CallsStore::getAllCallers() const {
    ProcedureSet allCallers;
    for (const auto& pair : callsStore) {
        allCallers.insert(pair.first);
    }
    return allCallers;
}

CallsStore::ProcedureSet CallsStore::getCallees(const ProcedureName& caller) const {
    auto it = callsStore.find(caller);
    if (it != callsStore.end()) {
        return it->second;
    }
    return {};
}

CallsStore::ProcedureSet CallsStore::getCallers(const ProcedureName& callee) const {
    auto it = reversedCallsStore.find(callee);
    if (it != reversedCallsStore.end()) {
        return it->second;
    }
    return {};
}

void CallsStore::addCallsStar(const ProcedureName& caller, const ProcedureName& callee) {
    callsStarStore[caller].insert(callee);
    reversedCallsStarStore[callee].insert(caller);
}

bool CallsStore::hasCallsStar(const ProcedureName& caller, const ProcedureName& callee) const {
    auto it = callsStarStore.find(caller);
    return it != callsStarStore.end() && it->second.find(callee) != it->second.end();
}

CallsStore::ProcedureSet CallsStore::getAllCallsStarCallees() const {
    ProcedureSet allCallees;
    for (const auto& pair : callsStarStore) {
        allCallees.insert(pair.second.begin(), pair.second.end());
    }
    return allCallees;
}

CallsStore::ProcedureSet CallsStore::getAllCallsStarCallers() const {
    ProcedureSet allCallers;
    for (const auto& pair : callsStarStore) {
        allCallers.insert(pair.first);
    }
    return allCallers;
}

CallsStore::ProcedureSet CallsStore::getCallsStarCallees(const ProcedureName& caller) const {
    auto it = callsStarStore.find(caller);
    if (it != callsStarStore.end()) {
        return it->second;
    }
    return {};
}

CallsStore::ProcedureSet CallsStore::getCallsStarCallers(const ProcedureName& callee) const {
    auto it = reversedCallsStarStore.find(callee);
    if (it != reversedCallsStarStore.end()) {
        return it->second;
    }
    return {};
}
