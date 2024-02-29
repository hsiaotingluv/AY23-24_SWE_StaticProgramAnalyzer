#pragma once

#include "qps/evaluators/results_table.hpp"

namespace qps {
class ClauseEvaluator {
  public:
    virtual ~ClauseEvaluator() = default;

    [[nodiscard]] virtual auto evaluate() const -> OutputTable = 0;
};
} // namespace qps