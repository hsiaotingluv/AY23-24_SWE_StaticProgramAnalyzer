#include "../include/pkb/abstraction/OneToManyStore.h"

template<class KeyType, class ValueType>
OneToManyStore<KeyType, ValueType>::OneToManyStore() = default;

template<class KeyType, class ValueType>
void OneToManyStore<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    forward_map[key].insert(value);
    reverse_map[value].insert(key);
}

template<class KeyType, class ValueType>
bool OneToManyStore<KeyType, ValueType>::contains(const KeyType& key, const ValueType& value) const {
    auto it = forward_map.find(key);
    return it != forward_map.end() && it->second.find(value) != it->second.end();
}

template<class KeyType, class ValueType>
bool OneToManyStore<KeyType, ValueType>::containsKey(const KeyType& key) const {
    return forward_map.find(key) != forward_map.end();
}

template<class KeyType, class ValueType>
bool OneToManyStore<KeyType, ValueType>::containsValue(const ValueType& value) const {
    return reverse_map.find(value) != reverse_map.end();
}

template<class KeyType, class ValueType>
std::unordered_set<ValueType> OneToManyStore<KeyType, ValueType>::getValuesByKey(const KeyType& key) const {
    if (forward_map.find(key) != forward_map.end()) {
        return forward_map.at(key);
    }
    return {};
}

template<class KeyType, class ValueType>
std::unordered_set<KeyType> OneToManyStore<KeyType, ValueType>::getKeysByValue(const ValueType& value) const {
    if (reverse_map.find(value) != reverse_map.end()) {
        return reverse_map.at(value);
    }
    return {};
}

template<class KeyType, class ValueType>
std::unordered_map<KeyType, std::unordered_set<ValueType>> OneToManyStore<KeyType, ValueType>::getAll() const {
    return forward_map;
}

template<class KeyType, class ValueType>
std::unordered_set<KeyType> OneToManyStore<KeyType, ValueType>::getAllKeys() const {
    std::unordered_set<KeyType> keys;
    for (const auto& pair : forward_map) {
        keys.insert(pair.first);
    }
    return keys;
}

template<class KeyType, class ValueType>
std::unordered_set<ValueType> OneToManyStore<KeyType, ValueType>::getAllValues() const {
    std::unordered_set<ValueType> values;
    for (const auto& pair : reverse_map) {
        values.insert(pair.first);
    }
    return values;
}