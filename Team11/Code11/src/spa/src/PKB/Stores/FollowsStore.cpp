#include "PKB/Stores/FollowsStore.h"

FollowsStore::FollowsStore() = default;

void FollowsStore::addFollows(const StatementNumber& s1, const StatementNumber& s2) {
    followsStore[s1] = s2;
    reversedFollowsStore[s2] = s1;

    followsStarStore[s1].insert(s2);
    reversedFollowsStarStore[s2].insert(s1);
}

bool FollowsStore::hasFollows(const StatementNumber& s1, const StatementNumber& s2) const {
    auto it = followsStore.find(s1);
    return it != followsStore.end() && it->second == s2;
}

FollowsStore::StatementToStatementMap FollowsStore::getAllFollows() const {
    return followsStore;
}

FollowsStore::StatementSet FollowsStore::getAllFollowsKeys() const {
    StatementSet keys;
    for (const auto& pair : followsStore) {
        keys.insert(pair.first);
    }
    return keys;
}

FollowsStore::StatementSet FollowsStore::getAllFollowsValues() const {
    StatementSet values;
    for (const auto& pair : followsStore) {
        values.insert(pair.second);
    }
    return values;
}

FollowsStore::StatementNumber FollowsStore::getFollowsFollowing(const StatementNumber& s) const {
    auto it = followsStore.find(s);
    if (it != followsStore.end()) {
        return it->second;
    }
    return "";
}

FollowsStore::StatementNumber FollowsStore::getFollowsBy(const StatementNumber& s) const {
    auto it = reversedFollowsStore.find(s);
    if (it != reversedFollowsStore.end()) {
        return it->second;
    }
    return "";
}

void FollowsStore::addFollowsStars(const StatementNumber& s1, const StatementNumber& s2) {
    followsStarStore[s1].insert(s2);
    reversedFollowsStarStore[s2].insert(s1);
}

bool FollowsStore::hasFollowsStars(const StatementNumber& s1, const StatementNumber& s2) const {
    if (followsStarStore.find(s1) == followsStarStore.end()) {
        return false;
    }
    return followsStarStore.at(s1).find(s2) != followsStarStore.at(s1).end();
}

FollowsStore::StatementToSetMap FollowsStore::getAllFollowsStar() const {
    return followsStarStore;
};

FollowsStore::StatementSet FollowsStore::getAllFollowsStarKeys() const {
    StatementSet keys;
    for (const auto& pair : followsStarStore) {
        keys.insert(pair.first);
    }
    return keys;
}

FollowsStore::StatementSet FollowsStore::getAllFollowsStarValues() const {
    StatementSet values;
    for (const auto& pair : followsStarStore) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
}

std::unordered_set<FollowsStore::StatementNumber>
FollowsStore::getFollowsStarsFollowing(const StatementNumber& s) const {
    if (followsStarStore.find(s) == followsStarStore.end()) {
        return {};
    }
    return followsStarStore.at(s);
}

std::unordered_set<FollowsStore::StatementNumber> FollowsStore::getFollowsStarsBy(const StatementNumber& s) const {
    if (reversedFollowsStarStore.find(s) == reversedFollowsStarStore.end()) {
        return {};
    }
    return reversedFollowsStarStore.at(s);
}
