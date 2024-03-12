#pragma once
#include "catch.hpp"

#include <algorithm>

template <typename T>
auto require_equal(const std::vector<T>& actual, const std::vector<T>& expected) -> void {
    REQUIRE(actual.size() == expected.size());
    for (const auto& value : actual) {
        auto it = std::find(expected.begin(), expected.end(), value);
        REQUIRE(it != expected.end());
    }
}