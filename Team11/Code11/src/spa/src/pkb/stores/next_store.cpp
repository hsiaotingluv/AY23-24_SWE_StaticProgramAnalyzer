#include "pkb/stores/next_store.h"

NextStore::NextStore() = default;

void NextStore::add_next(const StatementNumber& before, const StatementNumber& after) {
    next_store[before].insert(after);
    reversed_next_store[after].insert(before);

    next_star_store[before].insert(after);
    reversed_next_star_store[after].insert(before);
}

bool NextStore::has_next(const StatementNumber& before, const StatementNumber& after) const {
    auto iter = next_store.find(before);
    return iter != next_store.end() && iter->second.find(after) != iter->second.end();
}

NextStore::StatementToStatementSetMap NextStore::get_all_next() const {
    return next_store;
}

NextStore::StatementSet NextStore::get_all_next_keys() const {
    StatementSet allNexts;
    for (const auto& pair : next_store) {
        allNexts.insert(pair.first);
    }
    return allNexts;
}

NextStore::StatementSet NextStore::get_all_next_values() const {
    StatementSet values;
    for (const auto& pair : next_store) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
}

NextStore::StatementSet NextStore::get_next(const StatementNumber& before) const {
    auto iter = next_store.find(before);
    if (iter != next_store.end()) {
        return iter->second;
    }
    return {};
}

NextStore::StatementSet NextStore::get_next_previous(const StatementNumber& after) const {
    auto iter = reversed_next_store.find(after);
    if (iter != reversed_next_store.end()) {
        return iter->second;
    }
    return {};
}

void NextStore::add_next_star(const StatementNumber& before, const StatementNumber& after) {
    next_star_store[before].insert(after);
    reversed_next_star_store[after].insert(before);
}

bool NextStore::has_next_star(const StatementNumber& before, const StatementNumber& after) const {
    auto iter = next_star_store.find(before);
    return iter != next_star_store.end() && iter->second.find(after) != iter->second.end();
}

NextStore::StatementToStatementSetMap NextStore::get_all_next_star() const {
    return next_star_store;
}

NextStore::StatementSet NextStore::get_all_next_star_keys() const {
    StatementSet allNexts;
    for (const auto& pair : next_store) {
        allNexts.insert(pair.first);
    }
    return allNexts;
}

NextStore::StatementSet NextStore::get_all_next_star_values() const {
    StatementSet values;
    for (const auto& pair : next_store) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
}

NextStore::StatementSet NextStore::get_next_star(const StatementNumber& before) const {
    auto iter = next_star_store.find(before);
    if (iter != next_star_store.end()) {
        return iter->second;
    }
    return {};
}

NextStore::StatementSet NextStore::get_next_star_previous(const StatementNumber& after) const {
    auto iter = reversed_next_star_store.find(after);
    if (iter != reversed_next_star_store.end()) {
        return iter->second;
    }
    return {};
}
