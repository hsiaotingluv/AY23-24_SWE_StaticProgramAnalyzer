#include <algorithm>
#include <queue>
#include <tuple>
#include <vector>

#include "pkb/stores/parent_store.h"

ParentStore::ParentStore() = default;

void ParentStore::add_parent(const StatementNumber& parent, const StatementNumber& child) {
    // Add to direct parent-child store
    parent_store.add(parent, child);
}

void ParentStore::populate_parent_and_reverse_parent_star() {
    // Extract all the parent relationships into a vector of tuples
    std::vector<std::tuple<StatementNumber, StatementNumber>> parent_relationships;
    for (const auto& pair : parent_store.getAll()) {
        for (const auto& child : pair.second) {
            parent_relationships.emplace_back(pair.first, child);
        }
    }

    // Sort parent_relationships by the second element of the tuple
    std::sort(parent_relationships.begin(), parent_relationships.end(), [](const auto& a, const auto& b) {
        return stoi(std::get<1>(a)) < stoi(std::get<1>(b));
    });

    // Populate parent_star_store and reversed_parent_star_store, starting from the last element of
    // parent_relationships
    for (auto it = parent_relationships.rbegin(); it != parent_relationships.rend(); ++it) {
        const auto& [parent, child] = *it;
        parent_star_store.add(parent, child);

        // Add all the parent_star relationships from child to parent
        for (const auto& grandchild : parent_star_store.getValuesByKey(child)) {
            parent_star_store.add(parent, grandchild);
        }
    }
}

bool ParentStore::has_parent(const StatementNumber& parent, const StatementNumber& child) const {
    return parent_store.contains(parent, child);
}

ParentStore::StatementToSetMap ParentStore::get_all_parent() const {
    return parent_store.getAll();
}

ParentStore::StatementSet ParentStore::get_all_parent_keys() const {
    return parent_store.getAllKeys();
}

ParentStore::StatementSet ParentStore::get_all_parent_values() const {
    return parent_store.getAllValues();
}

ParentStore::StatementSet ParentStore::get_parent_children(const StatementNumber& parent) const {
    return parent_store.getValuesByKey(parent);
}

ParentStore::StatementNumber ParentStore::get_parent(const StatementNumber& child) const {
    auto parentSet = parent_store.getKeysByValue(child);
    if (!parentSet.empty()) {
        return *parentSet.begin();
    }
    return "";
}

bool ParentStore::has_parent_star(const StatementNumber& parent, const StatementNumber& child) const {
    return parent_star_store.contains(parent, child);
}

ParentStore::StatementToSetMap ParentStore::get_all_parent_star() const {
    return parent_star_store.getAll();
}

ParentStore::StatementSet ParentStore::get_all_parent_star_keys() const {
    return parent_star_store.getAllKeys();
}

ParentStore::StatementSet ParentStore::get_all_parent_star_values() const {
    return parent_star_store.getAllValues();
}

ParentStore::StatementSet ParentStore::get_parent_star_children(const StatementNumber& parent) const {
    return parent_star_store.getValuesByKey(parent);
}

ParentStore::StatementSet ParentStore::get_star_parent(const StatementNumber& child) const {
    return parent_star_store.getKeysByValue(child);
}
