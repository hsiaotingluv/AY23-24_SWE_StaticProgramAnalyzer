#include "pkb/stores/follows_store.h"
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

FollowsStore::FollowsStore() = default;

void FollowsStore::add_follows(const StatementNumber& s1, const StatementNumber& s2) {
    follows_store.add(s1, s2);
}

void FollowsStore::populate_follows_and_reverse_follows_star() {
    // extract all the follows relationships into a vector of tuples
    std::vector<std::tuple<StatementNumber, StatementNumber>> follows_relationships;
    for (const auto& pair : follows_store.getAll()) {
        follows_relationships.emplace_back(pair.first, pair.second);
    }

    // sort follows_relationships by the second element of the tuple
    std::sort(follows_relationships.begin(), follows_relationships.end(), [](const auto& a, const auto& b) {
        return stoi(std::get<1>(a)) < stoi(std::get<1>(b));
    });

    // populate follows_star_store and reversed_follows_star_store, starting from the last element of
    // follows_relationships
    for (auto it = follows_relationships.rbegin(); it != follows_relationships.rend(); ++it) {
        const auto& [s1, s2] = *it;
        follows_star_store.add(s1, s2);

        // add all the follows_star relationships from s2 to s1
        for (const auto& s3 : follows_star_store.getValuesByKey(s2)) {
            follows_star_store.add(s1, s3);
        }
    }
}

bool FollowsStore::has_follows(const StatementNumber& s1, const StatementNumber& s2) const {
    return follows_store.contains(s1, s2);
}

FollowsStore::StatementToStatementMap FollowsStore::get_all_follows() const {
    return follows_store.getAll();
}

FollowsStore::StatementSet FollowsStore::get_all_follows_keys() const {
    return follows_store.getAllKeys();
}

FollowsStore::StatementSet FollowsStore::get_all_follows_values() const {
    return follows_store.getAllValues();
}

FollowsStore::StatementNumber FollowsStore::get_follows_following(const StatementNumber& s) const {
    return follows_store.getValueByKey(s);
}

FollowsStore::StatementNumber FollowsStore::get_follows_by(const StatementNumber& s) const {
    return follows_store.getKeyByValue(s);
}

bool FollowsStore::has_follows_stars(const StatementNumber& s1, const StatementNumber& s2) const {
    return follows_star_store.contains(s1, s2);
}

FollowsStore::StatementToSetMap FollowsStore::get_all_follows_star() const {
    return follows_star_store.getAll();
}

FollowsStore::StatementSet FollowsStore::get_all_follows_star_keys() const {
    return follows_star_store.getAllKeys();
}

FollowsStore::StatementSet FollowsStore::get_all_follows_star_values() const {
    return follows_star_store.getAllValues();
}

std::unordered_set<FollowsStore::StatementNumber>
FollowsStore::get_follows_stars_following(const StatementNumber& s) const {
    return follows_star_store.getValuesByKey(s);
}

std::unordered_set<FollowsStore::StatementNumber> FollowsStore::get_follows_stars_by(const StatementNumber& s) const {
    return follows_star_store.getKeysByValue(s);
}
