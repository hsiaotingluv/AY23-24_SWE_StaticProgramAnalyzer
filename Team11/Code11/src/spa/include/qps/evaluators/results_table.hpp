#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>
#include <variant>
#include <vector>

namespace qps {

struct UnitTable {};

class Table {
    std::vector<std::shared_ptr<Synonym>> record_type;
    std::vector<std::vector<std::string>> record_value;

  public:
    Table() = default;

    Table(const std::vector<std::shared_ptr<Synonym>>& column_keys) : record_type(column_keys) {
    }

    auto add_row(const std::vector<std::string>& record) -> void {
        record_value.push_back(record);
    }

    [[nodiscard]] auto get_column() const -> std::vector<std::shared_ptr<Synonym>> {
        return record_type;
    }

    [[nodiscard]] auto get_records() const -> std::vector<std::vector<std::string>> {
        return record_value;
    }

    [[nodiscard]] auto get_column() -> std::vector<std::shared_ptr<Synonym>>& {
        return record_type;
    }

    [[nodiscard]] auto get_records() -> std::vector<std::vector<std::string>>& {
        return record_value;
    }

    [[nodiscard]] auto empty() const -> bool {
        return record_type.empty() || record_value.empty();
    }
};

using OutputTable = std::variant<Table, UnitTable>;

auto is_empty(const OutputTable& table) -> bool;
auto is_unit(const OutputTable& table) -> bool;

template <class T>
void reorder(std::vector<T>& v, std::vector<int> const& order) {
    std::vector<T> v_copy(v.size());
    for (size_t s = 0; s < order.size(); ++s) {
        v_copy[s] = v[order[s]];
    }

    v = v_copy;
}

auto join(const OutputTable& table1, const OutputTable& table2) -> OutputTable;
auto project(const std::shared_ptr<pkb::ReadFacade>& read_facade, const OutputTable& table, const Reference& reference)
    -> std::vector<std::string>;
void print(const Table& table);

namespace detail {
// Helper functions for join
auto sort_and_get_order(std::vector<std::shared_ptr<Synonym>>& column_names) -> std::vector<int>;

// Column merge strategies
auto double_pointer_merge(std::vector<std::shared_ptr<Synonym>> column1, std::vector<std::shared_ptr<Synonym>> column2)
    -> std::vector<std::shared_ptr<Synonym>>;
auto unordered_set_merge(const std::vector<std::shared_ptr<Synonym>>& column1,
                         const std::vector<std::shared_ptr<Synonym>>& column2) -> std::vector<std::shared_ptr<Synonym>>;
auto ordered_set_merge(const std::vector<std::shared_ptr<Synonym>>& column1,
                       const std::vector<std::shared_ptr<Synonym>>& column2) -> std::vector<std::shared_ptr<Synonym>>;

// Join strategies
auto cross_join_with_conflict_checks(const Table& table1, const Table& table2) -> OutputTable;

auto cross_join(const Table& table1, const Table& table2) -> OutputTable;
auto cross_join(Table&& table1, Table&& table2) -> OutputTable;

auto merge_join(const Table& table1, const Table& table2) -> OutputTable;
// auto merge_join(Table&& table1, Table&& table2) -> OutputTable;

auto cross_merge_join(Table&& table1, Table&& table2) -> OutputTable;

} // namespace detail
} // namespace qps