#pragma once

#include "one_to_one_store.h"

template <typename KeyType, typename ValueType>
OneToOneStore<KeyType, ValueType>::OneToOneStore() = default;

template <typename KeyType, typename ValueType>
void OneToOneStore<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    forward_map.insert({key, value});
    reverse_map.insert({value, key});
}

template <typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::contains_key_val_pair(const KeyType& key, const ValueType& value) const {
    auto it = forward_map.find(key);
    return it != forward_map.end() && it->second == value;
}

template <typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::contains_key(const KeyType& key) const {
    return forward_map.find(key) != forward_map.end();
}

template <typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::contains_val(const ValueType& value) const {
    return reverse_map.find(value) != reverse_map.end();
}

template <typename KeyType, typename ValueType>
ValueType OneToOneStore<KeyType, ValueType>::get_val_by_key(const KeyType& key) const {
    auto it = forward_map.find(key);
    return it != forward_map.end() ? it->second : ValueType();
}

template <typename KeyType, typename ValueType>
KeyType OneToOneStore<KeyType, ValueType>::get_key_by_val(const ValueType& value) const {
    auto it = reverse_map.find(value);
    return it != reverse_map.end() ? it->second : KeyType();
}

template <typename KeyType, typename ValueType>
std::unordered_map<KeyType, ValueType> OneToOneStore<KeyType, ValueType>::get_all() const {
    return forward_map;
}

template <class KeyType, class ValueType>
std::unordered_set<std::tuple<KeyType, ValueType>> OneToOneStore<KeyType, ValueType>::get_all_pairs() const {
    std::unordered_set<std::tuple<KeyType, ValueType>> allPairs;

    for (const auto& [key, val] : forward_map) {
        allPairs.insert(std::make_tuple(key, val));
    }

    return allPairs;
}

template <typename KeyType, typename ValueType>
std::unordered_set<KeyType> OneToOneStore<KeyType, ValueType>::get_all_keys() const {
    std::unordered_set<KeyType> keys;
    for (const auto& pair : forward_map) {
        keys.insert(pair.first);
    }
    return keys;
}

template <typename KeyType, typename ValueType>
std::unordered_set<ValueType> OneToOneStore<KeyType, ValueType>::get_all_vals() const {
    std::unordered_set<ValueType> values;
    for (const auto& pair : reverse_map) {
        values.insert(pair.first);
    }
    return values;
}