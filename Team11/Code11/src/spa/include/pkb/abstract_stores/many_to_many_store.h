#pragma once

#include "common/hashable_tuple.h"

#include <unordered_map>
#include <unordered_set>

template <class KeyType, class ValueType>
class ManyToManyStore {
  public:
    using Key = KeyType;
    using Value = ValueType;
    ManyToManyStore();

    void add(const KeyType& key, const ValueType& value);
    bool has_relationship() const;
    bool contains_key_val_pair(const KeyType& key, const ValueType& value) const;
    bool contains_key(const KeyType& key) const;
    bool contains_val(const ValueType& value) const;
    std::unordered_set<ValueType> get_vals_by_key(const KeyType& key) const;
    std::unordered_set<KeyType> get_keys_by_val(const ValueType& value) const;
    std::unordered_map<KeyType, std::unordered_set<ValueType>> get_all() const;
    std::unordered_map<ValueType, std::unordered_set<KeyType>> get_all_reverse() const;
    std::unordered_set<std::tuple<KeyType, ValueType>> get_all_pairs() const;
    std::unordered_set<KeyType> get_all_keys() const;
    std::unordered_set<ValueType> get_all_vals() const;

  private:
    std::unordered_map<KeyType, std::unordered_set<ValueType>> forward_map;
    std::unordered_map<ValueType, std::unordered_set<KeyType>> reverse_map;
};

#include "many_to_many_store.tpp"
