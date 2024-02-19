#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/results_table.hpp"
#include "qps/parser/entities/clause.hpp"
#include <memory>

namespace qps {
auto pattern_clause_evaluator(const std::shared_ptr<ReadFacade>& read_facade,
                              const std::shared_ptr<PatternClause>& pattern_clause) -> std::optional<Table>;
} // namespace qps
