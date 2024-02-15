#include "pkb/stores/follows_store.h"
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

FollowsStore::FollowsStore() = default;

void FollowsStore::add_follows(const StatementNumber& s1, const StatementNumber& s2) {
    follows_store[s1] = s2;
    reversed_follows_store[s2] = s1;
}

void FollowsStore::populate_follows_and_reverse_follows_star() {
    // extract all the follows relationships into a vector of tuples
    std::vector<std::tuple<StatementNumber, StatementNumber>> follows_relationships;
    for (const auto& pair : follows_store) {
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
        follows_star_store[s1].insert(s2);
        reversed_follows_star_store[s2].insert(s1);

        // add all the follows_star relationships from s2 to s1
        if (follows_star_store.find(s2) != follows_star_store.end()) {
            for (const auto& s3 : follows_star_store.at(s2)) {
                follows_star_store[s1].insert(s3);
                reversed_follows_star_store[s3].insert(s1);
            }
        }
    }
}

bool FollowsStore::has_follows(const StatementNumber& s1, const StatementNumber& s2) const {
    auto it = follows_store.find(s1);
    return it != follows_store.end() && it->second == s2;
}

FollowsStore::StatementToStatementMap FollowsStore::get_all_follows() const {
    return follows_store;
}

FollowsStore::StatementSet FollowsStore::get_all_follows_keys() const {
    StatementSet keys;
    for (const auto& pair : follows_store) {
        keys.insert(pair.first);
    }
    return keys;
}

FollowsStore::StatementSet FollowsStore::get_all_follows_values() const {
    StatementSet values;
    for (const auto& pair : follows_store) {
        values.insert(pair.second);
    }
    return values;
}

FollowsStore::StatementNumber FollowsStore::get_follows_following(const StatementNumber& s) const {
    auto it = follows_store.find(s);
    if (it != follows_store.end()) {
        return it->second;
    }
    return "";
}

FollowsStore::StatementNumber FollowsStore::get_follows_by(const StatementNumber& s) const {
    auto it = reversed_follows_store.find(s);
    if (it != reversed_follows_store.end()) {
        return it->second;
    }
    return "";
}

bool FollowsStore::has_follows_stars(const StatementNumber& s1, const StatementNumber& s2) const {
    if (follows_star_store.find(s1) == follows_star_store.end()) {
        return false;
    }
    return follows_star_store.at(s1).find(s2) != follows_star_store.at(s1).end();
}

FollowsStore::StatementToSetMap FollowsStore::get_all_follows_star() const {
    return follows_star_store;
}

FollowsStore::StatementSet FollowsStore::get_all_follows_star_keys() const {
    StatementSet keys;
    for (const auto& pair : follows_star_store) {
        keys.insert(pair.first);
    }
    return keys;
}

FollowsStore::StatementSet FollowsStore::get_all_follows_star_values() const {
    StatementSet values;
    for (const auto& pair : follows_star_store) {
        values.insert(pair.second.begin(), pair.second.end());
    }
    return values;
}

std::unordered_set<FollowsStore::StatementNumber>
FollowsStore::get_follows_stars_following(const StatementNumber& s) const {
    if (follows_star_store.find(s) == follows_star_store.end()) {
        return {};
    }
    return follows_star_store.at(s);
}

std::unordered_set<FollowsStore::StatementNumber> FollowsStore::get_follows_stars_by(const StatementNumber& s) const {
    if (reversed_follows_star_store.find(s) == reversed_follows_star_store.end()) {
        return {};
    }
    return reversed_follows_star_store.at(s);
}
