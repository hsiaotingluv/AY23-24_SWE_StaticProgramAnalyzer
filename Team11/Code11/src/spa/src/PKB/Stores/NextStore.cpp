#include "PKB/Stores/NextStore.h"

NextStore::NextStore() = default;

void NextStore::addNext(const StatementNumber& before, const StatementNumber& after) {
    nextStore[before].insert(after);
    reversedNextStore[after].insert(before);

    nextStarStore[before].insert(after);
    reversedNextStarStore[after].insert(before);
}

bool NextStore::hasNext(const StatementNumber& before, const StatementNumber& after) const {
    auto iter = nextStore.find(before);
    return iter != nextStore.end() && iter->second.find(after) != iter->second.end();
}

NextStore::StatementToStatementSetMap NextStore::getAllNext() const {
    return nextStore;
}

NextStore::StatementSet NextStore::getAllNextKeys() const {
    StatementSet allNexts;
    for (const auto& pair : nextStore) {
        allNexts.insert(pair.first);
    }
    return allNexts;
}

NextStore::StatementSet NextStore::getAllNextValues() const {
    StatementSet values;
    for (const auto& pair : nextStore) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
}

NextStore::StatementSet NextStore::getNext(const StatementNumber& before) const {
    auto iter = nextStore.find(before);
    if (iter != nextStore.end()) {
        return iter->second;
    }
    return {};
}

NextStore::StatementSet NextStore::getNextPrevious(const StatementNumber& after) const {
    auto iter = reversedNextStore.find(after);
    if (iter != reversedNextStore.end()) {
        return iter->second;
    }
    return {};
}

void NextStore::addNextStar(const StatementNumber& before, const StatementNumber& after) {
    nextStarStore[before].insert(after);
    reversedNextStarStore[after].insert(before);
}

bool NextStore::hasNextStar(const StatementNumber& before, const StatementNumber& after) const {
    auto iter = nextStarStore.find(before);
    return iter != nextStarStore.end() && iter->second.find(after) != iter->second.end();
}

NextStore::StatementToStatementSetMap NextStore::getAllNextStar() const {
    return nextStarStore;
}

NextStore::StatementSet NextStore::getAllNextStarKeys() const {
    StatementSet allNexts;
    for (const auto& pair : nextStore) {
        allNexts.insert(pair.first);
    }
    return allNexts;
}

NextStore::StatementSet NextStore::getAllNextStarValues() const {
    StatementSet values;
    for (const auto& pair : nextStore) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
}

NextStore::StatementSet NextStore::getNextStar(const StatementNumber& before) const {
    auto iter = nextStarStore.find(before);
    if (iter != nextStarStore.end()) {
        return iter->second;
    }
    return {};
}

NextStore::StatementSet NextStore::getNextStarPrevious(const StatementNumber& after) const {
    auto iter = reversedNextStarStore.find(after);
    if (iter != reversedNextStarStore.end()) {
        return iter->second;
    }
    return {};
}
