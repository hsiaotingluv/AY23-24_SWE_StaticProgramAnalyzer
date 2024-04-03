#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/template_utils.hpp"

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
auto such_that_clause_evaluator_selector(const std::shared_ptr<pkb::ReadFacade>& read_facade, const bool is_negated) {
    return overloaded{

        [read_facade, is_negated](const qps::Follows& follows) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<FollowsEvaluator>(read_facade, follows, is_negated);
        },

        [read_facade, is_negated](const qps::FollowsT& follows_t) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<FollowsTEvaluator>(read_facade, follows_t, is_negated);
        },

        [read_facade, is_negated](const qps::Parent& parent) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ParentEvaluator>(read_facade, parent, is_negated);
        },

        [read_facade, is_negated](const qps::ParentT& parent) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ParentTEvaluator>(read_facade, parent, is_negated);
        },

        [read_facade, is_negated](const qps::UsesS& uses_s) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<UsesSEvaluator>(read_facade, uses_s, is_negated);
        },

        [read_facade, is_negated](const qps::UsesP& uses_p) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<UsesPEvaluator>(read_facade, uses_p, is_negated);
        },

        [read_facade, is_negated](const qps::ModifiesS& modifies_s) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ModifiesSEvaluator>(read_facade, modifies_s, is_negated);
        },

        [read_facade, is_negated](const qps::ModifiesP& modifies_p) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ModifiesPEvaluator>(read_facade, modifies_p, is_negated);
        },

        [read_facade, is_negated](const qps::Calls& calls) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<CallsEvaluator>(read_facade, calls, is_negated);
        },

        [read_facade, is_negated](const qps::CallsT& calls_t) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<CallsTEvaluator>(read_facade, calls_t, is_negated);
        },

        [read_facade, is_negated](const qps::Next& next) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<NextEvaluator>(read_facade, next, is_negated);
        },

        [read_facade, is_negated](const qps::NextT& calls) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<NextTEvaluator>(read_facade, calls, is_negated);
        },

        [read_facade, is_negated](const qps::Affects&) -> std::shared_ptr<ClauseEvaluator> {
            // TODO: Implement Affects here
            return nullptr;
        }};
}
} // namespace qps
