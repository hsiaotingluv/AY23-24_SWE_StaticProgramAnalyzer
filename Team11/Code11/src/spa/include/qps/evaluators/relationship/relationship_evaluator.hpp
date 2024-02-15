#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/modifiesS_evaluator.hpp"
#include "qps/evaluators/results_map.hpp"
#include "qps/template_utils.hpp"

#include "qps/parser/entities/relationship.hpp"
#include <memory>
#include <optional>

namespace qps {
const auto relationship_evaluator = [](std::shared_ptr<ReadFacade> read_facade, ResultsMap& results_map) {
    return overloaded{[read_facade, &results_map](const qps::ModifiesS& modifies) -> std::optional<Table> {
                          const auto syn1 = modifies.stmt;
                          const auto syn2 = modifies.ent;

                          return std::visit(ModifiesS_evaluator(read_facade), syn1, syn2);
                      },
                      [](const auto& x) -> std::optional<Table> {
                          return std::nullopt;
                      }};
};
}