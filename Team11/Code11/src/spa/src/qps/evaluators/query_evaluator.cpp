#include "qps/evaluators/query_evaluator.hpp"
#include "qps/evaluators/relationship/clause_evaluator_selector.hpp"
#include "qps/evaluators/results_table.hpp"
#include "qps/evaluators/with_evaluator.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"

#include <memory>
#include <variant>
#include <vector>

namespace qps {

auto QueryEvaluator::evaluate(const qps::Query& query_obj) -> std::vector<std::string> {
    const auto reference = query_obj.reference;

    auto curr_table = OutputTable{UnitTable{}};

    if (query_obj.clauses.empty()) {
        // Short-circuit if there are no clauses
        return project(read_facade, curr_table, query_obj.reference);
    }

    // Step 1: populate all synonyms
    for (const auto& clause : query_obj.clauses) {
        if (const auto such_that_clause = std::dynamic_pointer_cast<qps::SuchThatClause>(clause)) {
            const auto relationship = such_that_clause->rel_ref;
            evaluator = std::visit(clause_evaluator_selector(read_facade), relationship);
        } else if (const auto pattern_clause = std::dynamic_pointer_cast<qps::PatternClause>(clause)) {
            const auto syntactic_pattern = pattern_clause->syntactic_pattern;
            evaluator = std::visit(clause_evaluator_selector(read_facade), syntactic_pattern);
        } else if (const auto with_clause = std::dynamic_pointer_cast<qps::WithClause>(clause)) {
            evaluator = std::make_shared<WithEvaluator>(read_facade, with_clause->ref1, with_clause->ref2);
        }

        if (evaluator == nullptr) {
#ifdef DEBUG
            std::cerr << "Failed to create evaluator for clause: " << *clause << std::endl;
#endif
            auto empty_table = OutputTable{Table{}};
            return project(read_facade, empty_table, query_obj.reference);
        }

        auto next_table = evaluator->evaluate();
        if (is_empty(next_table)) {
#ifdef DEBUG
            std::cerr << "Failed to evaluate clause: " << *clause << std::endl;
#endif
            return project(read_facade, next_table, query_obj.reference);
        }

        curr_table = join(std::move(curr_table), std::move(next_table));
        if (is_empty(curr_table)) {
#ifdef DEBUG
            std::cerr << "Conflict detected for clause: " << *clause << std::endl;
#endif
            return project(read_facade, curr_table, query_obj.reference);
        }
    }

    // Step 2: project to relevant synonym
    return project(read_facade, curr_table, query_obj.reference);
};

} // namespace qps