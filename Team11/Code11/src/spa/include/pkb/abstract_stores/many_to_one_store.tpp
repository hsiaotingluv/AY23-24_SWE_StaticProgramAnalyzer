#pragma once

#include "many_to_one_store.h"

template <class KeyType, class ValueType>
ManyToOneStore<KeyType, ValueType>::ManyToOneStore() = default;

template <class KeyType, class ValueType>
void ManyToOneStore<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    forward_map.insert({key, value});
    reverse_map[value].insert(key);
}

template <typename KeyType, typename ValueType>
ValueType ManyToOneStore<KeyType, ValueType>::get_val_by_key(const KeyType& key) const {
    auto it = forward_map.find(key);
    return it != forward_map.end() ? it->second : ValueType();
}

template <class KeyType, class ValueType>
std::unordered_set<KeyType> ManyToOneStore<KeyType, ValueType>::get_keys_by_val(const ValueType& value) const {
    if (reverse_map.find(value) != reverse_map.end()) {
        return reverse_map.at(value);
    }
    return {};
}

template <class KeyType, class ValueType>
std::unordered_set<KeyType> ManyToOneStore<KeyType, ValueType>::get_all_keys() const {
    std::unordered_set<KeyType> keys;
    for (const auto& pair : forward_map) {
        keys.insert(pair.first);
    }
    return keys;
}