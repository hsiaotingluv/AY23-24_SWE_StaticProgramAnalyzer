#include "qps/evaluators/query_evaluator.hpp"
#include "qps/evaluators/entities/entity_scanner.hpp"
#include "qps/evaluators/relationship/clause_evaluator_selector.hpp"
#include "qps/evaluators/relationship/pattern_evaluator.hpp"
#include "qps/evaluators/results_table.hpp"

#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>

namespace qps {

auto QueryEvaluator::evaluate(const qps::Query& query_obj) -> std::vector<std::string> {
    const auto reference = query_obj.reference;
    const auto results = scan_entities(read_facade, reference);

    auto curr_table = Table{{reference}};
    for (const auto& result : results) {
        curr_table.add_row({result});
    }

    if (query_obj.clauses.empty()) {
        // Short-circuit if there are no clauses
        return {results.begin(), results.end()};
    }

    // Step 1: populate all synonyms
    for (const auto& clause : query_obj.clauses) {

        if (const auto such_that_clause = std::dynamic_pointer_cast<qps::SuchThatClause>(clause)) {
            const auto relationship = such_that_clause->rel_ref;
            evaluator = std::visit(clause_evaluator_selector(read_facade), relationship);
        } else if (const auto pattern_clause = std::dynamic_pointer_cast<qps::PatternClause>(clause)) {
            evaluator = std::make_shared<PatternEvaluator>(read_facade, *pattern_clause);
        }

        const auto maybe_table = evaluator->evaluate();
        if (!maybe_table.has_value()) {
            return {};
        }

        const auto& next_table = maybe_table.value();
        const auto maybe_new_table = join(curr_table, next_table);
        if (!maybe_new_table.has_value()) {
            // Conflict detected -> no results
            return {};
        }
        curr_table = maybe_new_table.value();
    }

    // Step 2: project to relevant synonym
    return project(read_facade, curr_table, query_obj.reference);
};

} // namespace qps