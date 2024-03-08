#pragma once

#include "one_to_many_store.h"

template <class KeyType, class ValueType>
OneToManyStore<KeyType, ValueType>::OneToManyStore() = default;

template <class KeyType, class ValueType>
void OneToManyStore<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    forward_map[key].insert(value);
    reverse_map.insert({value, key});
}

template <class KeyType, class ValueType>
bool OneToManyStore<KeyType, ValueType>::contains_key_val_pair(const KeyType& key, const ValueType& value) const {
    auto it = forward_map.find(key);
    return it != forward_map.end() && it->second.find(value) != it->second.end();
}

template <class KeyType, class ValueType>
bool OneToManyStore<KeyType, ValueType>::contains_key(const KeyType& key) const {
    return forward_map.find(key) != forward_map.end();
}

template <class KeyType, class ValueType>
bool OneToManyStore<KeyType, ValueType>::contains_val(const ValueType& value) const {
    return reverse_map.find(value) != reverse_map.end();
}

template <class KeyType, class ValueType>
std::unordered_set<ValueType> OneToManyStore<KeyType, ValueType>::get_vals_by_key(const KeyType& key) const {
    if (forward_map.find(key) != forward_map.end()) {
        return forward_map.at(key);
    }
    return {};
}

template <class KeyType, class ValueType>
KeyType OneToManyStore<KeyType, ValueType>::get_key_by_val(const ValueType& value) const {
    auto it = reverse_map.find(value);
    return it != reverse_map.end() ? it->second : KeyType();
}

template <class KeyType, class ValueType>
std::unordered_map<KeyType, std::unordered_set<ValueType>> OneToManyStore<KeyType, ValueType>::get_all() const {
    return forward_map;
}

template <class KeyType, class ValueType>
std::unordered_set<std::tuple<KeyType, ValueType>> OneToManyStore<KeyType, ValueType>::get_all_pairs() const {
    std::unordered_set<std::tuple<KeyType, ValueType>> allPairs;

    for (const auto& [key, val] : forward_map) {
        for (const auto& v : val) {
            allPairs.insert(std::make_tuple(key, v));
        }
    }

    return allPairs;
}

template <class KeyType, class ValueType>
std::unordered_set<KeyType> OneToManyStore<KeyType, ValueType>::get_all_keys() const {
    std::unordered_set<KeyType> keys;
    for (const auto& pair : forward_map) {
        keys.insert(pair.first);
    }
    return keys;
}

template <class KeyType, class ValueType>
std::unordered_set<ValueType> OneToManyStore<KeyType, ValueType>::get_all_vals() const {
    std::unordered_set<ValueType> values;
    for (const auto& pair : reverse_map) {
        values.insert(pair.first);
    }
    return values;
}