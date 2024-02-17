#pragma once

#include "pkb/facades/read_facade.h"
#include "modifies_s_evaluator.hpp"
#include "qps/template_utils.hpp"

#include "qps/parser/entities/relationship.hpp"
#include "parent_evaluator.hpp"

namespace qps {
    auto clause_evaluator_selector(const std::shared_ptr<ReadFacade> read_facade) {
        return overloaded {

            [read_facade](qps::ModifiesS& modifies) -> std::shared_ptr<ClauseEvaluator> {
                return std::make_shared<ModifiesSEvaluator>(read_facade, modifies);
            },

            [read_facade](qps::Parent& parent) -> std::shared_ptr<ClauseEvaluator> {
                return std::make_shared<ParentEvaluator>(read_facade, parent);
            },

            // TODO: add other clause evaluator cases here

            [](const auto& x) -> std::shared_ptr<ClauseEvaluator> {
                return nullptr;
            }
        };
    };
} // namespace qps