#pragma once

#include <unordered_map>
#include <unordered_set>

template <class KeyType, class ValueType>
class OneToManyStore {
  public:
    OneToManyStore();

    void add(const KeyType& key, const ValueType& value);
    bool contains(const KeyType& key, const ValueType& value) const;
    bool containsKey(const KeyType& key) const;
    bool containsValue(const ValueType& value) const;
    std::unordered_set<ValueType> getValuesByKey(const KeyType& key) const;
    std::unordered_set<KeyType> getKeysByValue(const ValueType& value) const;
    std::unordered_map<KeyType, std::unordered_set<ValueType>> getAll() const;
    std::unordered_set<std::tuple<KeyType, ValueType>> getAllPairs() const;
    std::unordered_set<KeyType> getAllKeys() const;
    std::unordered_set<ValueType> getAllValues() const;

  private:
    std::unordered_map<KeyType, std::unordered_set<ValueType>> forward_map;
    std::unordered_map<ValueType, std::unordered_set<KeyType>> reverse_map;
};

#include "../../../src/pkb/abstraction/OneToManyStore.tpp"
