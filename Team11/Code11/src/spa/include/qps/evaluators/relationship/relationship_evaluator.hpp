#pragma once

#include "PKB/Facades/ReadFacade.h"
#include "qps/evaluators/relationship/modifiesS_evaluator.hpp"
#include "qps/evaluators/results_map.hpp"
#include "qps/evaluators/type_traits.hpp"

#include "qps/parser/entities/relationship.hpp"
#include <memory>

namespace qps {
const auto relationship_evaluator = [](std::shared_ptr<ReadFacade> readFacade, ResultsMap& results_map) {
    return overloaded{[readFacade, &results_map](const qps::ModifiesS& modifies) {
                          const auto syn1 = modifies.stmt;
                          const auto syn2 = modifies.ent;

                          return std::visit(ModifiesS_evaluator(readFacade, results_map), syn1, syn2);
                      },
                      [](const auto& x) {
                          return;
                      }};
};
}