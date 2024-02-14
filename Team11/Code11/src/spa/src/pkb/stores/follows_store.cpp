#include "pkb/stores/follows_store.h"

FollowsStore::FollowsStore() = default;

void FollowsStore::add_follows(const StatementNumber& s1, const StatementNumber& s2) {
    follows_store[s1] = s2;
    reversed_follows_store[s2] = s1;

    follows_star_store[s1].insert(s2);
    reversed_follows_star_store[s2].insert(s1);
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

void FollowsStore::add_follows_stars(const StatementNumber& s1, const StatementNumber& s2) {
    follows_star_store[s1].insert(s2);
    reversed_follows_star_store[s2].insert(s1);
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
