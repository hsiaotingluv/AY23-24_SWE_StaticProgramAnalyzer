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
    std::vector<T> v_copy(order.size());
    for (size_t s = 0; s < order.size(); ++s) {
        v_copy[s] = v[order[s]];
    }

    v = v_copy;
}

auto subtract(OutputTable&& table1, OutputTable&& table2, const std::shared_ptr<pkb::ReadFacade>& read_facade) -> Table;
auto join(OutputTable&& table1, OutputTable&& table2) -> OutputTable;
auto project_to_table(const std::shared_ptr<pkb::ReadFacade>& read_facade, OutputTable& table,
                      const Reference& reference, bool should_transform = false) -> OutputTable;
auto project(const std::shared_ptr<pkb::ReadFacade>& read_facade, OutputTable& table, const Reference& reference)
    -> std::vector<std::string>;
void print(const Table& table);

class AttributeExtractor {
    std::shared_ptr<pkb::ReadFacade> read_facade;

  public:
    AttributeExtractor(std::shared_ptr<pkb::ReadFacade> read_facade) : read_facade(std::move(read_facade)) {
    }

    [[nodiscard]] auto operator()() const {
        auto read_facade = this->read_facade; // Prevent stack-use-after-scope
        return overloaded{
            [read_facade](const std::shared_ptr<Synonym>&) -> std::function<std::string(const std::string&)> {
                return [](const std::string& x) -> std::string {
                    return x;
                };
            },
            [read_facade](const AttrRef& attr_ref) -> std::function<std::string(const std::string&)> {
                const auto synonym = attr_ref.synonym;
                if (std::dynamic_pointer_cast<PrintSynonym>(synonym) &&
                    std::holds_alternative<VarName>(attr_ref.attr_name)) {
                    return [read_facade](const std::string& x) -> std::string {
                        return *read_facade->get_vars_used_by_statement(x).begin();
                    };
                } else if (std::dynamic_pointer_cast<ReadSynonym>(synonym) &&
                           std::holds_alternative<VarName>(attr_ref.attr_name)) {
                    return [read_facade](const std::string& x) -> std::string {
                        return *read_facade->get_vars_modified_by_statement(x).begin();
                    };
                } else if (std::dynamic_pointer_cast<CallSynonym>(synonym) &&
                           std::holds_alternative<ProcName>(attr_ref.attr_name)) {
                    return [read_facade](const std::string& x) -> std::string {
                        return read_facade->get_procedure_name_called_by(x);
                    };
                } else {
                    return [](const std::string& x) -> std::string {
                        return x;
                    };
                }
            }};
    }
};

namespace detail {
// Helper functions for join
auto sort_and_get_order(std::vector<std::shared_ptr<Synonym>>& column_names) -> std::vector<int>;

// Column merge strategies
auto double_pointer_merge(std::vector<std::shared_ptr<Synonym>> column1, std::vector<std::shared_ptr<Synonym>> column2)
    -> std::vector<std::shared_ptr<Synonym>>;

// Join strategies
auto cross_join(Table&& table1, Table&& table2) -> OutputTable;
auto merge_join(Table&& table1, Table&& table2) -> OutputTable;
auto cross_merge_join(Table&& table1, Table&& table2) -> OutputTable;

} // namespace detail
} // namespace qps