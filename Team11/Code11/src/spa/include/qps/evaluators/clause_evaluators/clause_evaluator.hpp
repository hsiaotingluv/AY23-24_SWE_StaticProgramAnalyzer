#pragma once

#include "qps/evaluators/results_table.hpp"

namespace qps {
class ClauseEvaluator {
    bool is_negated;

    [[nodiscard]] virtual auto evaluate_positive() const -> OutputTable = 0;

    [[nodiscard]] virtual auto negate_result(OutputTable output) const -> OutputTable;

  protected:
    std::shared_ptr<pkb::ReadFacade> read_facade;

  public:
    virtual ~ClauseEvaluator() = default;

    ClauseEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, bool is_negated)
        : read_facade(std::move(read_facade)), is_negated(is_negated){};

    [[nodiscard]] auto evaluate() const -> OutputTable;
};
} // namespace qps