#include "pkb/stores/parent_store.h"

ParentStore::ParentStore() = default;

void ParentStore::add_parent(const StatementNumber& parent, const StatementNumber& child) {
    // Add to direct parent-child store
    parent_store[parent].insert(child);
    reversed_parent_store[child] = parent;

    // Also add to ParentStar store to maintain the invariant that Parent implies ParentStar
    parent_star_store[parent].insert(child);
    reversed_parent_star_store[child].insert(parent);
}

bool ParentStore::has_parent(const StatementNumber& parent, const StatementNumber& child) const {
    auto it = parent_store.find(parent);
    return it != parent_store.end() && it->second.find(child) != it->second.end();
}

ParentStore::StatementToSetMap ParentStore::get_all_parent() const {
    return parent_store;
}

ParentStore::StatementSet ParentStore::get_all_parent_keys() const {
    StatementSet keys;
    for (const auto& pair : parent_store) {
        keys.insert(pair.first);
    }
    return keys;
}

ParentStore::StatementSet ParentStore::get_all_parent_values() const {
    StatementSet values;
    for (const auto& pair : parent_store) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
};

ParentStore::StatementSet ParentStore::get_parent_children(const StatementNumber& parent) const {
    auto it = parent_store.find(parent);
    if (it != parent_store.end()) {
        return it->second;
    }
    return {};
}

ParentStore::StatementNumber ParentStore::get_parent(const StatementNumber& child) const {
    auto it = reversed_parent_store.find(child);
    if (it != reversed_parent_store.end()) {
        return it->second;
    }
    return ""; // Return an empty string to indicate no parent found
}

void ParentStore::add_parent_star(const StatementNumber& parent, const StatementNumber& child) {
    parent_star_store[parent].insert(child);
    reversed_parent_star_store[child].insert(parent);
}

bool ParentStore::has_parent_star(const StatementNumber& parent, const StatementNumber& child) const {
    auto it = parent_star_store.find(parent);
    return it != parent_star_store.end() && it->second.find(child) != it->second.end();
}

ParentStore::StatementToSetMap ParentStore::get_all_parent_star() const {
    return parent_star_store;
}

ParentStore::StatementSet ParentStore::get_all_parent_star_keys() const {
    StatementSet keys;
    for (const auto& pair : parent_star_store) {
        keys.insert(pair.first);
    }
    return keys;
}

ParentStore::StatementSet ParentStore::get_all_parent_star_values() const {
    StatementSet values;
    for (const auto& pair : parent_star_store) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
}

ParentStore::StatementSet ParentStore::get_parent_star_children(const StatementNumber& parent) const {
    auto it = parent_star_store.find(parent);
    if (it != parent_star_store.end()) {
        return it->second;
    }
    return {};
}

ParentStore::StatementSet ParentStore::get_star_parent(const StatementNumber& child) const {
    auto it = reversed_parent_star_store.find(child);
    if (it != reversed_parent_star_store.end()) {
        return it->second;
    }
    return {};
}
