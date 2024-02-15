#pragma once

#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace qps {

class Table {
    const std::vector<Synonym> record_type;
    std::vector<std::vector<uint32_t>> record_value;

  public:
    Table() = default;

    Table(const std::vector<Synonym>& column_keys) : record_type(column_keys) {
    }

    auto add_row(const std::vector<uint32_t>& record) -> void {
        record_value.push_back(record);
    }

    [[nodiscard]] auto get_column() const -> std::vector<Synonym> {
        return record_type;
    }

    [[nodiscard]] auto get_record() const -> std::vector<std::vector<uint32_t>> {
        return record_value;
    }
};

struct ResultsMap {
  private:
    std::unordered_map<Synonym, std::vector<std::string>> mapping;
    bool is_unsatisfiable_{false};
    bool is_valid_{false};

  public:
    // TODO: update_mapping assumes that there is no conflict --> will not be true if there are multiple clauses
    template <typename T, std::enable_if_t<is_variant_member_v<T, Synonym>, bool> = true>
    void update_mapping(const T& synonym, const std::string& value) {
        if (mapping.find(synonym) == mapping.end()) {
            mapping[synonym] = {value};
        } else {
            mapping[synonym].push_back(value);
        }
    }

    template <typename T, std::enable_if_t<is_variant_member_v<T, Synonym>, bool> = true>
    void update_mapping(const T& synonym, const std::unordered_set<std::string>& values) {
        if (mapping.find(synonym) == mapping.end()) {
            mapping[synonym] = {values.begin(), values.end()};
        } else {
            mapping[synonym].insert(mapping[synonym].end(), values.begin(), values.end());
        }
    }

    template <typename T>
    void update_mapping(const StmtSynonym& stmt_synonym, const T& value) {
        std::visit(
            [&](const auto& synonym) {
                update_mapping(synonym, value);
            },
            stmt_synonym);
    }

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

    [[nodiscard]] auto project(const Synonym& synonym) const -> std::vector<std::string> {
        if (mapping.find(synonym) == mapping.end()) {
            return {};
        }
        return mapping.at(synonym);
    }
};
} // namespace qps