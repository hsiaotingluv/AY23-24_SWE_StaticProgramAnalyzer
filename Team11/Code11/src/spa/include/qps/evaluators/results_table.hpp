#pragma once

#include "qps/parser/entities/synonym.hpp"
#include <memory>

#include <vector>

namespace qps {

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
};

} // namespace qps