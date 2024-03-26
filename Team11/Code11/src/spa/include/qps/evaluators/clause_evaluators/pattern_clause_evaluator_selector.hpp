#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/template_utils.hpp"

#include "qps/evaluators/clause_evaluators/pattern/pattern_assign_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/pattern/pattern_if_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/pattern/pattern_while_evaluator.hpp"

namespace qps {
auto pattern_clause_evaluator_selector(const std::shared_ptr<pkb::ReadFacade>& read_facade) {
    return overloaded{[read_facade](const qps::PatternAssign& pattern) -> std::shared_ptr<ClauseEvaluator> {
                          return std::make_shared<PatternAssignEvaluator>(read_facade, pattern);
                      },

                      [read_facade](const qps::PatternIf& pattern) -> std::shared_ptr<ClauseEvaluator> {
                          return std::make_shared<PatternIfEvaluator>(read_facade, pattern);
                      },

                      [read_facade](const qps::PatternWhile& pattern) -> std::shared_ptr<ClauseEvaluator> {
                          return std::make_shared<PatternWhileEvaluator>(read_facade, pattern);
                      }};
};
} // namespace qps
