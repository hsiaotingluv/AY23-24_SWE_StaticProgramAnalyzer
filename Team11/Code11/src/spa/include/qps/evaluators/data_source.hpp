#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/results_table.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <string>
#include <unordered_set>

namespace qps {

class DataSource {
    std::function<std::unordered_set<std::string>(const std::shared_ptr<Synonym>&)> getter_func;

  public:
    explicit DataSource(const std::shared_ptr<pkb::ReadFacade>& read_facade, const OutputTable& output_table)
        : getter_func(
              [read_facade, &output_table](const std::shared_ptr<Synonym>& synonym) -> std::unordered_set<std::string> {
                  if (is_unit(output_table) || is_empty(output_table)) {
#ifdef DEBUG
                      std::cerr << "[Miss]: empty table: " << synonym << std::endl;
#endif
                      return synonym->scan(read_facade);
                  }

                  const auto& table = std::get<Table>(output_table);
                  auto results = table.get_column_value(synonym);

                  if (results.empty()) {
#ifdef DEBUG
                      std::cerr << "[Miss]: missing synonym: " << synonym << std::endl;
#endif
                      return synonym->scan(read_facade);
                  }
#ifdef DEBUG
                  std::cerr << "[Hit]" << std::endl;
#endif
                  return results;
              }) {
    }

    DataSource(const std::shared_ptr<pkb::ReadFacade>& read_facade)
        : getter_func([read_facade](const std::shared_ptr<Synonym>& synonym) -> std::unordered_set<std::string> {
              return synonym->scan(read_facade);
          }) {
    }

    [[nodiscard]] auto get_data(const std::shared_ptr<Synonym>& synonym) const -> std::unordered_set<std::string> {
        return getter_func(synonym);
    }
};
} // namespace qps
