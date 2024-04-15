#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/data_source.hpp"
#include "qps/template_utils.hpp"

#include "qps/evaluators/clause_evaluators/pattern/pattern_assign_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/pattern/pattern_if_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/pattern/pattern_while_evaluator.hpp"

namespace qps {
auto pattern_clause_evaluator_selector(const DataSource& data_source,
                                       const std::shared_ptr<pkb::ReadFacade>& read_facade, bool is_negated) {
    return overloaded{
        [&data_source, read_facade, is_negated](const qps::PatternAssign& pattern) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<PatternAssignEvaluator>(data_source, read_facade, pattern, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::PatternIf& pattern) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<PatternIfEvaluator>(data_source, read_facade, pattern, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::PatternWhile& pattern) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<PatternWhileEvaluator>(data_source, read_facade, pattern, is_negated);
        }};
}
} // namespace qps
