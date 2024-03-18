#pragma once

#include "common/hashable_tuple.h"

#include <unordered_map>
#include <unordered_set>

template <class KeyType, class ValueType>
class ManyToOneStore {
  public:
    using Key = KeyType;
    using Value = ValueType;
    ManyToOneStore();

    void add(const KeyType& key, const ValueType& value);
    ValueType get_val_by_key(const KeyType& key) const;
    std::unordered_set<KeyType> get_keys_by_val(const ValueType& value) const;
    std::unordered_set<KeyType> get_all_keys() const;

  private:
    std::unordered_map<KeyType, ValueType> forward_map;
    std::unordered_map<ValueType, std::unordered_set<KeyType>> reverse_map;
};

#include "many_to_one_store.tpp"
