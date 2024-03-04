#pragma once

#include "catch.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"
#include <memory>
#include <type_traits>

template <typename T>
auto get(const std::shared_ptr<qps::Synonym>& synonym) {
    return std::dynamic_pointer_cast<T>(synonym);
}

template <typename T, std::enable_if_t<!std::is_base_of_v<qps::Synonym, T>, bool> = true>
void require_value(const std::shared_ptr<qps::Synonym>& synonym, std::string value) {
    REQUIRE(false);
}

template <typename T, std::enable_if_t<std::is_base_of_v<qps::Synonym, T>, bool> = true>
void require_value(const std::shared_ptr<qps::Synonym>& synonym, std::string value) {
    const auto result = get<T>(synonym);
    REQUIRE(result);
    REQUIRE(result->get_name() == value);
}

template <typename T, typename Variant,
          std::enable_if_t<qps::is_variant_member_v<std::shared_ptr<qps::Synonym>, Variant>, bool> = true>
void require_value(const Variant& reference, std::string value) {
    return std::visit(qps::overloaded{[&value](const std::shared_ptr<qps::Synonym>& synonym) {
                                          return require_value<T>(synonym, value);
                                      },
                                      [&value](const qps::Synonyms& synonyms) {
                                          auto is_matched = false;
                                          for (const auto& synonym : synonyms) {
                                              const auto result = get<T>(synonym);
                                              if (result && result->get_name() == value) {
                                                  is_matched = true;
                                                  break;
                                              }
                                          }
                                          REQUIRE(is_matched);
                                      },
                                      [](const auto& reference) {
                                          REQUIRE(false);
                                      }},
                      reference);
}

template <typename Variant, std::enable_if_t<!qps::is_variant_member_v<qps::BooleanReference, Variant>, bool> = true>
void require_boolean(const Variant& reference) {
    REQUIRE(false);
}

template <typename Variant, std::enable_if_t<qps::is_variant_member_v<qps::BooleanReference, Variant>, bool> = true>
void require_boolean(const Variant& reference) {
    return std::visit(qps::overloaded{[](const qps::BooleanReference& reference) {
                                          REQUIRE(true);
                                      },
                                      [](const auto& reference) {
                                          REQUIRE(false);
                                      }},
                      reference);
}
