#pragma once

#include "catch.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"
#include <memory>
#include <type_traits>

template <typename T>
auto get(const std::shared_ptr<qps::Synonym>& synonym) {
    return std::dynamic_pointer_cast<T>(synonym);
}

template <typename T, std::enable_if_t<std::is_base_of_v<qps::Synonym, T>, bool> = true>
void require_value(const std::shared_ptr<qps::Synonym>& synonym, std::string value) {
    const auto result = get<T>(synonym);
    REQUIRE(result);
    REQUIRE(result->get_name() == value);
}

template <typename T, typename Variant,
          std::enable_if_t<qps::is_variant_member_v<std::shared_ptr<qps::Synonym>, Variant>, bool> = true,
          std::enable_if_t<std::is_base_of_v<qps::Synonym, T>, bool> = true>
void require_value(const Variant& reference, std::string value) {
    if (std::holds_alternative<std::shared_ptr<qps::Synonym>>(reference)) {
        const auto synonym = std::get<std::shared_ptr<qps::Synonym>>(reference);
        return require_value<T>(synonym, value);
    } else if (std::holds_alternative<qps::Synonyms>(reference)) {
        const auto synonyms = std::get<qps::Synonyms>(reference);
        auto is_matched = false;
        for (const auto& synonym : synonyms) {
            const auto result = get<T>(synonym);
            if (result && result->get_name() == value) {
                is_matched = true;
                break;
            }
        }
        REQUIRE(is_matched);
        return;
    }
    REQUIRE(false);
}
