#include "qps/evaluators/query_evaluator.hpp"
#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator_selector.hpp"
#include "qps/evaluators/relationship/pattern_evaluator.hpp"
#include "qps/evaluators/results_table.hpp"
#include "qps/parser/semantic_analyser.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <variant>
#include <vector>

namespace qps {

auto build_table(const Reference& reference, const std::shared_ptr<ReadFacade>& read_facade) -> OutputTable {
    return std::visit(overloaded{
                          [&read_facade](const std::shared_ptr<Synonym>& synonym) -> OutputTable {
                              auto table = Table{{synonym}};
                              for (const auto& result : synonym->scan(read_facade)) {
                                  table.add_row({result});
                              }
                              return table;
                          },
                          [&read_facade](const Synonyms& synonyms) -> OutputTable {
                              auto table = Table{synonyms};
                              for (const auto& synonym : synonyms) {
                                  for (const auto& result : synonym->scan(read_facade)) {
                                      table.add_row({result});
                                  }
                              }
                              return table;
                          },
                          [&read_facade](const BooleanReference&) -> OutputTable {
                              return UnitTable{};
                          },
                      },
                      reference);
}

auto QueryEvaluator::evaluate(const qps::Query& query_obj) -> std::vector<std::string> {
    const auto reference = query_obj.reference;

    auto curr_table = build_table(reference, read_facade);

    if (query_obj.clauses.empty()) {
        // Short-circuit if there are no clauses
        return project(read_facade, curr_table, query_obj.reference);
    }

    // Step 1: populate all synonyms
    for (const auto& clause : query_obj.clauses) {
        if (const auto such_that_clause = std::dynamic_pointer_cast<qps::SuchThatClause>(clause)) {
            const auto relationship = such_that_clause->rel_ref;
            evaluator = std::visit(clause_evaluator_selector(read_facade), relationship);
        } else if (const auto pattern_clause = std::dynamic_pointer_cast<qps::PatternAssignClause>(clause)) {
            evaluator = std::make_shared<PatternEvaluator>(read_facade, *pattern_clause);
        }

        if (evaluator == nullptr) {
            std::cerr << "Failed to create evaluator for clause: " << *clause << std::endl;
            return {};
        }

        const auto next_table = evaluator->evaluate();
        if (is_empty(next_table)) {
            return {};
        }

        curr_table = join(curr_table, next_table);
        if (is_empty(curr_table)) {
            // Conflict detected -> no results
            return {};
        }
    }

    // Step 2: project to relevant synonym
    return project(read_facade, curr_table, query_obj.reference);
};

} // namespace qps