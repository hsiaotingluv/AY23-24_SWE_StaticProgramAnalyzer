#include "../include/pkb/abstraction/OneToOneStore.h"

template <typename KeyType, typename ValueType>
OneToOneStore<KeyType, ValueType>::OneToOneStore() = default;

template <typename KeyType, typename ValueType>
void OneToOneStore<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    forward_map[key] = value;
    reverse_map[value] = key;
}

template <typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::contains(const KeyType& key, const ValueType& value) const {
    auto it = forward_map.find(key);
    return it != forward_map.end() && it->second == value;
}

template <typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::containsKey(const KeyType& key) const {
    return forward_map.find(key) != forward_map.end();
}

template <typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::containsValue(const ValueType& value) const {
    return reverse_map.find(value) != reverse_map.end();
}

template <typename KeyType, typename ValueType>
ValueType OneToOneStore<KeyType, ValueType>::getValueByKey(const KeyType& key) const {
    auto it = forward_map.find(key);
    return it != forward_map.end() ? it->second : ValueType();
}

template <typename KeyType, typename ValueType>
KeyType OneToOneStore<KeyType, ValueType>::getKeyByValue(const ValueType& value) const {
    auto it = reverse_map.find(value);
    return it != reverse_map.end() ? it->second : KeyType();
}

template <typename KeyType, typename ValueType>
std::unordered_map<KeyType, ValueType> OneToOneStore<KeyType, ValueType>::getAll() const {
    return forward_map;
}

template <class KeyType, class ValueType>
std::unordered_set<std::tuple<KeyType, ValueType>> OneToOneStore<KeyType, ValueType>::getAllPairs() const {
    std::unordered_set<std::tuple<KeyType, ValueType>> allPairs;

    for (const auto& [key, val] : forward_map) {
        allPairs.insert(std::make_tuple(key, val));
    }

    return allPairs;
}

template <typename KeyType, typename ValueType>
std::unordered_set<KeyType> OneToOneStore<KeyType, ValueType>::getAllKeys() const {
    std::unordered_set<KeyType> keys;
    for (const auto& pair : forward_map) {
        keys.insert(pair.first);
    }
    return keys;
}

template <typename KeyType, typename ValueType>
std::unordered_set<ValueType> OneToOneStore<KeyType, ValueType>::getAllValues() const {
    std::unordered_set<ValueType> values;
    for (const auto& pair : reverse_map) {
        values.insert(pair.first);
    }
    return values;
}