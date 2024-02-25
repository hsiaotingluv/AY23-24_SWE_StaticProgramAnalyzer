#pragma once

#include "qps/evaluators/results_table.hpp"
#include <optional>

namespace qps {
class ClauseEvaluator {
  public:
    virtual ~ClauseEvaluator() = default;

    [[nodiscard]] virtual auto evaluate() const -> std::optional<Table> = 0;
};
} // namespace qps