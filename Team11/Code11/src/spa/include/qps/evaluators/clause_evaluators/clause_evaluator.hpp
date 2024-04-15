#pragma once

#include "qps/evaluators/data_source.hpp"
#include "qps/evaluators/results_table.hpp"

namespace qps {
class ClauseEvaluator {
    DataSource data_source;
    bool is_negated;

    [[nodiscard]] virtual auto evaluate_positive() const -> OutputTable = 0;

    [[nodiscard]] virtual auto negate_result(OutputTable output) const -> OutputTable;

  protected:
    std::shared_ptr<pkb::ReadFacade> read_facade;

    [[nodiscard]] auto get_data(const std::shared_ptr<Synonym>& synonym) const -> std::unordered_set<std::string> {
        return data_source.get_data(synonym);
    }

  public:
    explicit ClauseEvaluator(DataSource data_source, std::shared_ptr<pkb::ReadFacade> read_facade, bool is_negated)
        : data_source(std::move(data_source)), is_negated(is_negated), read_facade(std::move(read_facade)) {
    }

    virtual ~ClauseEvaluator() = default;

    [[nodiscard]] auto evaluate() const -> OutputTable;
};
} // namespace qps