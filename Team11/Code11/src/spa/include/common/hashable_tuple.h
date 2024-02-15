#pragma once

#include <functional>
#include <tuple>

template <typename T, typename X>
struct TupleHash {
    size_t operator()(const std::tuple<T, X>& t) const {
        return std::hash<T>()(std::get<0>(t)) ^ std::hash<X>()(std::get<1>(t));
    }
};

namespace std {
template <typename T, typename X>
struct hash<tuple<T, X>> {
    size_t operator()(const tuple<T, X>& t) const {
        return TupleHash<T, X>()(t);
    }
};
} // namespace std