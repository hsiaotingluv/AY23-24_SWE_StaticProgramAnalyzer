#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/data_source.hpp"
#include "qps/template_utils.hpp"

#include "qps/evaluators/clause_evaluators/relationship/affects_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/calls_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/calls_t_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/follows_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/follows_t_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/modifies_p_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/modifies_s_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/next_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/next_t_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/parent_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/parent_t_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/uses_p_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/relationship/uses_s_evaluator.hpp"

#include "qps/parser/entities/relationship.hpp"

namespace qps {
auto such_that_clause_evaluator_selector(const DataSource& data_source,
                                         const std::shared_ptr<pkb::ReadFacade>& read_facade, bool is_negated) {
    return overloaded{

        [&data_source, read_facade, is_negated](const qps::Follows& follows) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<FollowsEvaluator>(data_source, read_facade, follows, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::FollowsT& follows_t) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<FollowsTEvaluator>(data_source, read_facade, follows_t, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::Parent& parent) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ParentEvaluator>(data_source, read_facade, parent, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::ParentT& parent) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ParentTEvaluator>(data_source, read_facade, parent, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::UsesS& uses_s) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<UsesSEvaluator>(data_source, read_facade, uses_s, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::UsesP& uses_p) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<UsesPEvaluator>(data_source, read_facade, uses_p, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::ModifiesS& modifies_s) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ModifiesSEvaluator>(data_source, read_facade, modifies_s, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::ModifiesP& modifies_p) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ModifiesPEvaluator>(data_source, read_facade, modifies_p, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::Calls& calls) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<CallsEvaluator>(data_source, read_facade, calls, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::CallsT& calls_t) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<CallsTEvaluator>(data_source, read_facade, calls_t, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::Next& next) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<NextEvaluator>(data_source, read_facade, next, is_negated);
        },

        [&data_source, read_facade, is_negated](const qps::NextT& calls) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<NextTEvaluator>(data_source, read_facade, calls, is_negated);
        },

        [&data_source, read_facade](const qps::Affects& affects) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<AffectsEvaluator>(data_source, read_facade, affects, false);
        }};
}
} // namespace qps
