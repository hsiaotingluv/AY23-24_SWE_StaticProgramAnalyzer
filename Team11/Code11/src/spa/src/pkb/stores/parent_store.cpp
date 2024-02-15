#include <algorithm>
#include <queue>
#include <tuple>
#include <vector>

#include "pkb/stores/parent_store.h"

ParentStore::ParentStore() = default;

void ParentStore::add_parent(const StatementNumber& parent, const StatementNumber& child) {
    // Add to direct parent-child store
    parent_store[parent].insert(child);
    reversed_parent_store[child] = parent;
}

void ParentStore::populate_parent_and_reverse_parent_star() {
    // Extract all the parent relationships into a vector of tuples
    std::vector<std::tuple<StatementNumber, StatementNumber>> parent_relationships;
    for (const auto& pair : parent_store) {
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
        parent_star_store[parent].insert(child);
        reversed_parent_star_store[child].insert(parent);

        // Add all the parent_star relationships from child to parent
        if (parent_star_store.find(child) != parent_star_store.end()) {
            for (const auto& grandchild : parent_star_store.at(child)) {
                parent_star_store[parent].insert(grandchild);
                reversed_parent_star_store[grandchild].insert(parent);
            }
        }
    }
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
