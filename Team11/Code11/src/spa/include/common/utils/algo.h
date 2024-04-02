#pragma once

#include <unordered_map>
#include <vector>

struct OrderingBySecondElement {
    template <class T>
    auto operator()(const T& a, const T& b) const -> bool {
        return stoi(std::get<1>(a)) < stoi(std::get<1>(b));
    }
};

template <class I>
class OrderingByIndexMap {
    std::unordered_map<I, unsigned long> order_map;

  public:
    explicit OrderingByIndexMap(const std::vector<I>& order) {
        for (unsigned long i = 0; i < order.size(); i++) {
            order_map.insert({order[i], i});
        }
    }

    template <class T>
    auto operator()(const T& a, const T& b) const -> bool {
        return order_map.at(std::get<1>(a)) > order_map.at(std::get<1>(b));
    }
};