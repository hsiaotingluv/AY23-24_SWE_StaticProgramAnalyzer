#pragma once

#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"
#include <memory>
#include <type_traits>

template <typename T>
auto get(const std::shared_ptr<qps::Synonym>& synonym) {
    return std::dynamic_pointer_cast<T>(synonym);
}

template <typename T, std::enable_if_t<std::is_base_of_v<qps::Synonym, T>, bool> = true>
auto require_value(const std::shared_ptr<qps::Synonym>& synonym, std::string value) {
    const auto result = get<T>(synonym);
    REQUIRE(result);
    REQUIRE(result->get_name() == value);
    return result;
}

template <typename T, typename Variant,
          std::enable_if_t<qps::is_variant_member_v<std::shared_ptr<qps::Synonym>, Variant>, bool> = true,
          std::enable_if_t<std::is_base_of_v<qps::Synonym, T>, bool> = true>
auto require_value(const Variant& reference, std::string value) {
    REQUIRE(std::holds_alternative<std::shared_ptr<qps::Synonym>>(reference));

    const auto synonym = std::get<std::shared_ptr<qps::Synonym>>(reference);
    const auto result = get<T>(synonym);
    REQUIRE(result);
    REQUIRE(result->get_name() == value);
    return result;
}
