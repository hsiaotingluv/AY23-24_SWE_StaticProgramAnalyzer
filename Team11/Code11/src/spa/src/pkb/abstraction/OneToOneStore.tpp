template<typename KeyType, typename ValueType>
OneToOneStore<KeyType, ValueType>::OneToOneStore() = default;

template<typename KeyType, typename ValueType>
OneToOneStore<KeyType, ValueType>::~OneToOneStore() = default;

void OneToOneStore<KeyType, ValueType>::add(const KeyType& key, const ValueType& value) {
    forwardMap[key] = value;
    reverseMap[value] = key;
}

template<typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::contains(const KeyType& key, const ValueType& value) const {
    auto it = forwardMap.find(key);
    return it != forwardMap.end() && it->second == value;
}

template<typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::containsKey(const KeyType& key) const {
    return forwardMap.find(key) != forwardMap.end();
}

template<typename KeyType, typename ValueType>
bool OneToOneStore<KeyType, ValueType>::containsValue(const ValueType& value) const {
    return reverseMap.find(value) != reverseMap.end();
}

template<typename KeyType, typename ValueType>
ValueType OneToOneStore<KeyType, ValueType>::getValueByKey(const KeyType& key) const {
    auto it = forwardMap.find(key);
    return it != forwardMap.end() ? it->second : ValueType();
}

template<typename KeyType, typename ValueType>
KeyType OneToOneStore<KeyType, ValueType>::getKeyByValue(const ValueType& value) const {
    auto it = reverseMap.find(value);
    return it != reverseMap.end() ? it->second : KeyType();
}

template<typename KeyType, typename ValueType>
std::unordered_set<KeyType> OneToOneStore<KeyType, ValueType>::getAllKeys() const {
    std::unordered_set<KeyType> keys;
    for (const auto& pair : forwardMap) {
        keys.insert(pair.first);
    }
    return keys;
}

template<typename KeyType, typename ValueType>
std::unordered_set<ValueType> OneToOneStore<KeyType, ValueType>::getAllValues() const {
    std::unordered_set<ValueType> values;
    for (const auto& pair : reverseMap) {
        values.insert(pair.first);
    }
    return values;
}
