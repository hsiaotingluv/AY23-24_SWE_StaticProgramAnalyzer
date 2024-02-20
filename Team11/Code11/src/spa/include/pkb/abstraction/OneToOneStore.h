#pragma once

#include <unordered_map>
#include <unordered_set>

template<class KeyType, class ValueType>
class OneToOneStore {
  public:
    OneToOneStore();

    void add(const KeyType& key, const ValueType& value);
    bool contains(const KeyType& key, const ValueType& value) const;
    bool containsKey(const KeyType& key) const;
    bool containsValue(const ValueType& value) const;
    ValueType getValueByKey(const KeyType& key) const;
    KeyType getKeyByValue(const ValueType& value) const;
    std::unordered_map<KeyType, ValueType> getAll() const;
    std::unordered_set<KeyType> getAllKeys() const;
    std::unordered_set<ValueType> getAllValues() const;

  private:
    std::unordered_map<KeyType, ValueType> forward_map;
    std::unordered_map<ValueType, KeyType> reverse_map;
};

#include "../../../src/pkb/abstraction/OneToOneStore.tpp"
