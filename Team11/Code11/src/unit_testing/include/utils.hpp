#pragma once

#include "qps/parser/entities/synonym.hpp"

template <typename T>
auto get(const std::shared_ptr<qps::Synonym>& synonym) {
    return std::dynamic_pointer_cast<T>(synonym);
}

template <typename T>
auto require_value(const std::shared_ptr<qps::Synonym>& synonym, std::string value) {
    const auto result = get<T>(synonym);
    REQUIRE(result);
    REQUIRE(result->get_name() == value);
    return result;
}
