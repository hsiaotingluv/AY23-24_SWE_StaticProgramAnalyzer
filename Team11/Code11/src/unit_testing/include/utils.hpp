#pragma once

#include "catch.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/attribute.hpp"
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
auto require_value_bool(const std::shared_ptr<qps::Synonym>& synonym, std::string value) -> bool {
    const auto result = get<T>(synonym);
    return result && result->get_name() == value;
}

template <typename T, std::enable_if_t<std::is_base_of_v<qps::Synonym, T>, bool> = true>
void require_value(const std::shared_ptr<qps::Synonym>& synonym, std::string value) {
    REQUIRE(require_value_bool<T>(synonym, value));
}

template <typename T, typename Variant,
          std::enable_if_t<qps::is_variant_member_v<std::shared_ptr<qps::Synonym>, Variant>, bool> = true>
auto require_value_bool(const Variant& reference, std::string value) -> bool {
    return std::visit(qps::overloaded{[&value](const std::shared_ptr<qps::Synonym>& synonym) {
                                          return require_value_bool<T>(synonym, value);
                                      },
                                      [&value](const qps::Synonyms& synonyms) {
                                          for (const auto& synonym : synonyms) {
                                              if (require_value_bool<T>(synonym, value)) {
                                                  return true;
                                              }
                                          }
                                          return false;
                                      },
                                      [](const auto& reference) {
                                          return false;
                                      }},
                      reference);
}

template <typename T, typename Variant,
          std::enable_if_t<qps::is_variant_member_v<std::shared_ptr<qps::Synonym>, Variant>, bool> = true>
void require_value(const Variant& reference, std::string value) {
    REQUIRE(require_value_bool<T>(reference, value));
}

template <typename T, std::enable_if_t<std::is_base_of_v<qps::Synonym, T>, bool> = true>
void require_value(const qps::Reference& reference, std::string value) {
    return std::visit(qps::overloaded{[&value](const qps::BooleanReference&) {
                                          REQUIRE(false);
                                      },
                                      [&value](const auto& references) {
                                          for (const auto& reference : references) {
                                              if (require_value_bool<T>(reference, value)) {
                                                  REQUIRE(true);
                                                  return;
                                              }
                                          }
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

template <typename T>
auto require_value_bool(const qps::Elem& reference, T value) -> bool {
    return std::visit(qps::overloaded{[&value](const std::shared_ptr<qps::Synonym>& synonym) -> bool {
                                          if constexpr (std::is_base_of_v<qps::Synonym, T>) {
                                              return require_value_bool<T>(synonym, value);
                                          } else {
                                              return false;
                                          }
                                      },
                                      [&value](const qps::AttrRef& attr_ref) -> bool {
                                          if constexpr (std::is_same_v<T, qps::AttrRef>) {
                                              return attr_ref == value;
                                          } else {
                                              return false;
                                          }
                                      }},
                      reference);
}

template <typename T>
auto require_value(const qps::Reference& reference, T value) {
    return std::visit(qps::overloaded{[&value](const qps::BooleanReference& reference) {
                                          if constexpr (std::is_same_v<T, qps::BooleanReference>) {
                                              require_boolean(reference);
                                          } else {
                                              REQUIRE(false);
                                          }
                                      },
                                      [&value](const auto& references) {
                                          for (const auto& reference : references) {
                                              if (require_value_bool<T>(reference, value)) {
                                                  REQUIRE(true);
                                                  return;
                                              }
                                          }
                                          REQUIRE(false);
                                      }},
                      reference);
}
