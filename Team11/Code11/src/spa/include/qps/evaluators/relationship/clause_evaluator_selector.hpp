#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/template_utils.hpp"

#include "qps/evaluators/relationship/follows_evaluator.hpp"
#include "qps/evaluators/relationship/follows_t_evaluator.hpp"
#include "qps/evaluators/relationship/modifies_s_evaluator.hpp"
#include "qps/evaluators/relationship/parent_evaluator.hpp"
#include "qps/evaluators/relationship/parent_t_evaluator.hpp"
#include "qps/evaluators/relationship/pattern_assign_evaluator.hpp"
#include "qps/evaluators/relationship/uses_s_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
auto clause_evaluator_selector(const std::shared_ptr<pkb::ReadFacade>& read_facade) {
    return overloaded{

        [read_facade](const qps::Follows& follows) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<FollowsEvaluator>(read_facade, follows);
        },

        [read_facade](const qps::FollowsT& follows_t) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<FollowsTEvaluator>(read_facade, follows_t);
        },

        [read_facade](const qps::Parent& parent) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ParentEvaluator>(read_facade, parent);
        },

        [read_facade](const qps::ParentT& parent) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ParentTEvaluator>(read_facade, parent);
        },

        [read_facade](const qps::UsesS& uses_s) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<UsesSEvaluator>(read_facade, uses_s);
        },

        [read_facade](const qps::ModifiesS& modifies) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ModifiesSEvaluator>(read_facade, modifies);
        },

        // TODO: move this to a separate file
        [read_facade](const qps::PatternAssign& pattern) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<PatternAssignEvaluator>(read_facade, pattern);
        },

        // TODO: add other clause evaluator cases here
        [](const auto&) -> std::shared_ptr<ClauseEvaluator> {
            return nullptr;
        }};
};
} // namespace qps