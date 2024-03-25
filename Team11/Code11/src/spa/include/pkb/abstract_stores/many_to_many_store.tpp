#pragma once

#include "many_to_many_store.h"

template <class KeyType, class ValueType>
ManyToManyStore<KeyType, ValueType>::ManyToManyStore() = default;

template <class KeyType, class ValueType>
void ManyToManyStore<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    forward_map[key].insert(value);
    reverse_map[value].insert(key);
}

template <class KeyType, class ValueType>
bool ManyToManyStore<KeyType, ValueType>::has_relationship() const {
    return !forward_map.empty();
}

template <class KeyType, class ValueType>
bool ManyToManyStore<KeyType, ValueType>::contains_key_val_pair(const KeyType& key, const ValueType& value) const {
    auto it = forward_map.find(key);
    return it != forward_map.end() && it->second.find(value) != it->second.end();
}

template <class KeyType, class ValueType>
bool ManyToManyStore<KeyType, ValueType>::contains_key(const KeyType& key) const {
    return forward_map.find(key) != forward_map.end();
}

template <class KeyType, class ValueType>
bool ManyToManyStore<KeyType, ValueType>::contains_val(const ValueType& value) const {
    return reverse_map.find(value) != reverse_map.end();
}

template <class KeyType, class ValueType>
std::unordered_set<ValueType> ManyToManyStore<KeyType, ValueType>::get_vals_by_key(const KeyType& key) const {
    if (forward_map.find(key) != forward_map.end()) {
        return forward_map.at(key);
    }
    return {};
}

template <class KeyType, class ValueType>
std::unordered_set<KeyType> ManyToManyStore<KeyType, ValueType>::get_keys_by_val(const ValueType& value) const {
    if (reverse_map.find(value) != reverse_map.end()) {
        return reverse_map.at(value);
    }
    return {};
}

template <class KeyType, class ValueType>
std::unordered_map<KeyType, std::unordered_set<ValueType>> ManyToManyStore<KeyType, ValueType>::get_all() const {
    return forward_map;
}

template <class KeyType, class ValueType>
std::unordered_map<ValueType, std::unordered_set<KeyType>>
ManyToManyStore<KeyType, ValueType>::get_all_reverse() const {
    return reverse_map;
}

template <class KeyType, class ValueType>
std::unordered_set<std::tuple<KeyType, ValueType>> ManyToManyStore<KeyType, ValueType>::get_all_pairs() const {
    std::unordered_set<std::tuple<KeyType, ValueType>> allPairs;

    for (const auto& [key, val] : forward_map) {
        for (const auto& v : val) {
            allPairs.insert(std::make_tuple(key, v));
        }
    }

    return allPairs;
}

template <class KeyType, class ValueType>
std::unordered_set<KeyType> ManyToManyStore<KeyType, ValueType>::get_all_keys() const {
    std::unordered_set<KeyType> keys;
    for (const auto& pair : forward_map) {
        keys.insert(pair.first);
    }
    return keys;
}

template <class KeyType, class ValueType>
std::unordered_set<ValueType> ManyToManyStore<KeyType, ValueType>::get_all_vals() const {
    std::unordered_set<ValueType> values;
    for (const auto& pair : reverse_map) {
        values.insert(pair.first);
    }
    return values;
}