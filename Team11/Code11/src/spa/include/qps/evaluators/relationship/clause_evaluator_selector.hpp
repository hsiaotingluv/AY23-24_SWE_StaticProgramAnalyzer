#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/template_utils.hpp"

#include "qps/parser/entities/relationship.hpp"
#include "parent_evaluator.hpp"
#include "follows_evaluator.hpp"
#include "follows_t_evaluator.hpp"
#include "modifies_s_evaluator.hpp"
#include "uses_s_evaluator.hpp"
#include "parent_t_evaluator.hpp"

namespace qps {
auto clause_evaluator_selector(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded {

        [read_facade](qps::Follows& follows) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<FollowsEvaluator>(read_facade, follows);
        },

        [read_facade](qps::FollowsT& follows_t) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<FollowsTEvaluator>(read_facade, follows_t);
        },

        [read_facade](qps::Parent& parent) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ParentEvaluator>(read_facade, parent);
        },

        [read_facade](qps::ParentT& parent) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ParentTEvaluator>(read_facade, parent);
        },

        [read_facade](qps::UsesS& uses_s) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<UsesSEvaluator>(read_facade, uses_s);
        },

        [read_facade](qps::ModifiesS& modifies) -> std::shared_ptr<ClauseEvaluator> {
            return std::make_shared<ModifiesSEvaluator>(read_facade, modifies);
        },


        // TODO: add other clause evaluator cases here

        [](const auto& x) -> std::shared_ptr<ClauseEvaluator> {
            return nullptr;
        }
    };
};
} // namespace qps