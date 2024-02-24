#pragma once

#include "qps/evaluators/results_table.hpp"
#include <optional>

namespace qps {
class ClauseEvaluator {
  public:
    virtual ~ClauseEvaluator() = default;

    virtual auto evaluate() -> std::optional<Table> = 0;
};
} // namespace qps