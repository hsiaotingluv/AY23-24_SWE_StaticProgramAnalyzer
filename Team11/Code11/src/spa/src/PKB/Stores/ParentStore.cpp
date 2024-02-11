#include "PKB/Stores/ParentStore.h"

ParentStore::ParentStore() = default;

void ParentStore::addParent(const StatementNumber &parent, const StatementNumber &child) {
    // Add to direct parent-child store
    parentStore[parent].insert(child);
    reversedParentStore[child] = parent;

    // Also add to ParentStar store to maintain the invariant that Parent implies ParentStar
    parentStarStore[parent].insert(child);
    reversedParentStarStore[child].insert(parent);
}

bool ParentStore::hasParent(const StatementNumber &parent, const StatementNumber &child) const {
    auto it = parentStore.find(parent);
    return it != parentStore.end() && it->second.find(child) != it->second.end();
}

ParentStore::StatementToSetMap ParentStore::getAllParent() const {
    return parentStore;
};

ParentStore::StatementSet ParentStore::getAllParentKeys() const {
    StatementSet keys;
    for (const auto &pair: parentStore) {
        keys.insert(pair.first);
    }
    return keys;
};

ParentStore::StatementSet ParentStore::getAllParentValues() const {
    StatementSet values;
    for (const auto &pair: parentStore) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
};

ParentStore::StatementSet ParentStore::getParentChildren(const StatementNumber &parent) const {
    auto it = parentStore.find(parent);
    if (it != parentStore.end()) {
        return it->second;
    }
    return {};
}

ParentStore::StatementNumber ParentStore::getParent(const StatementNumber &child) const {
    auto it = reversedParentStore.find(child);
    if (it != reversedParentStore.end()) {
        return it->second;
    }
    return "";  // Return an empty string to indicate no parent found
}

void ParentStore::addParentStar(const StatementNumber &parent, const StatementNumber &child) {
    parentStarStore[parent].insert(child);
    reversedParentStarStore[child].insert(parent);
}

bool ParentStore::hasParentStar(const StatementNumber &parent, const StatementNumber &child) const {
    auto it = parentStarStore.find(parent);
    return it != parentStarStore.end() && it->second.find(child) != it->second.end();
}

ParentStore::StatementToSetMap ParentStore::getAllParentStar() const {
    return parentStarStore;
};

ParentStore::StatementSet ParentStore::getAllParentStarKeys() const {
    StatementSet keys;
    for (const auto &pair: parentStarStore) {
        keys.insert(pair.first);
    }
    return keys;
};

ParentStore::StatementSet ParentStore::getAllParentStarValues() const {
    StatementSet values;
    for (const auto &pair: parentStarStore) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
};

ParentStore::StatementSet ParentStore::getParentStarChildren(const StatementNumber &parent) const {
    auto it = parentStarStore.find(parent);
    if (it != parentStarStore.end()) {
        return it->second;
    }
    return {};
}

ParentStore::StatementSet ParentStore::getStarParent(const StatementNumber &child) const {
    auto it = reversedParentStarStore.find(child);
    if (it != reversedParentStarStore.end()) {
        return it->second;
    }
    return {};
}
