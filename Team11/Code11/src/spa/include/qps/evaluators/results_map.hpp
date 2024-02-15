#pragma once

#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace qps {
struct ResultsMap {
  private:
    std::unordered_map<std::shared_ptr<Synonym>, std::vector<std::string>> mapping;
    bool is_unsatisfiable_{false};
    bool is_valid_{false};

  public:
    // TODO: update_mapping assumes that there is no conflict --> will not be true if there are multiple clauses
    // template <typename T, std::enable_if_t<is_variant_member_v<T, Synonym>, bool> = true>
    // void update_mapping(const T& synonym, const std::string& value) {
    //     if (mapping.find(synonym) == mapping.end()) {
    //         mapping[synonym] = {value};
    //     } else {
    //         mapping[synonym].push_back(value);
    //     }
    // }

    // template <typename T, std::enable_if_t<is_variant_member_v<T, Synonym>, bool> = true>
    // void update_mapping(const T& synonym, const std::unordered_set<std::string>& values) {
    //     if (mapping.find(synonym) == mapping.end()) {
    //         mapping[synonym] = {values.begin(), values.end()};
    //     } else {
    //         mapping[synonym].insert(mapping[synonym].end(), values.begin(), values.end());
    //     }
    // }

    void update_mapping(const std::shared_ptr<Synonym>& synonym, const std::string& value) {
        if (mapping.find(synonym) == mapping.end()) {
            mapping[synonym] = {value};
        } else {
            mapping[synonym].push_back(value);
        }
    }

    void update_mapping(const std::shared_ptr<Synonym>& synonym, const std::unordered_set<std::string>& values) {
        if (mapping.find(synonym) == mapping.end()) {
            mapping[synonym] = {values.begin(), values.end()};
        } else {
            mapping[synonym].insert(mapping[synonym].end(), values.begin(), values.end());
        }
    }

    // template <typename T>
    // void update_mapping(const StmtSynonym& stmt_synonym, const T& value) {
    //     std::visit(
    //         [&](const auto& synonym) {
    //             update_mapping(synonym, value);
    //         },
    //         stmt_synonym);
    // }

    auto set_unsatisfiable() -> void {
        is_unsatisfiable_ = true;
    }

    auto set_valid() -> void {
        is_valid_ = true;
    }

    [[nodiscard]] auto is_unsatisfiable() const -> bool {
        return is_unsatisfiable_;
    }

    [[nodiscard]] auto is_valid() const -> bool {
        return is_valid_;
    }

    [[nodiscard]] auto project(const std::shared_ptr<Synonym>& synonym) const -> std::vector<std::string> {
        if (mapping.find(synonym) == mapping.end()) {
            return {};
        }
        return mapping.at(synonym);
    }
};
} // namespace qps