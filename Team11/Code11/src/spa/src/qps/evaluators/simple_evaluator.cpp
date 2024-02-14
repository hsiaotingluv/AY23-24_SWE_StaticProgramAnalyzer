#include "qps/evaluators/simple_evaluator.hpp"
#include "qps/evaluators/entities/entity_scanner.hpp"
#include "qps/evaluators/relationship/relationship_evaluator.hpp"

namespace qps {
auto Evaluator::evaluate(qps::Query query_obj) -> std::vector<std::string> {
    const auto reference = query_obj.reference;
    const auto results = std::visit(scan_entities(read_facade), reference);

    auto results_map = ResultsMap{};

    if (query_obj.clauses.empty()) {
        // Short-circuit if there are no clauses
        return {results.begin(), results.end()};
    }

    // Step 1: populate all synonyms
    for (const auto& clause : query_obj.clauses) {
        // TODO: currently we don't support any other clause types
        if (std::dynamic_pointer_cast<qps::SuchThatClause>(clause) == nullptr) {
            continue;
        }

        const auto such_that_clause = std::dynamic_pointer_cast<qps::SuchThatClause>(clause);
        const auto relationship = such_that_clause->rel_ref;

        std::visit(relationship_evaluator(read_facade, results_map), relationship);

        if (results_map.is_unsatisfiable()) {
            return {};
        } else if (results_map.is_valid()) {
            return {results.begin(), results.end()};
        }
    }

    // Step 2: project to relevant synonym
    return results_map.project(query_obj.reference);
};

} // namespace qps