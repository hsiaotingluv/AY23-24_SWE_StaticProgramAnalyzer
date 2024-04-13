#include "qps/evaluators/query_evaluator.hpp"
#include "qps/evaluators/clause_evaluators/pattern_clause_evaluator_selector.hpp"
#include "qps/evaluators/clause_evaluators/such_that_clause_evaluator_selector.hpp"
#include "qps/evaluators/clause_evaluators/with_evaluator.hpp"
#include "qps/evaluators/results_table.hpp"
#include "qps/optimisers/default.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"

#include <memory>
#include <variant>
#include <vector>

namespace qps {
auto QueryEvaluator::optimise(const Query& query_obj) const -> std::vector<Query> {
    const auto queries = std::vector<Query>{query_obj};
    return optimiser->optimise(queries);
}

auto QueryEvaluator::evaluate_query(const Query& query_obj) -> OutputTable {
    const auto reference = query_obj.reference;

    auto curr_table = OutputTable{UnitTable{}};

    if (query_obj.clauses.empty()) {
        // Short-circuit if there are no clauses
        return project_to_table(read_facade, curr_table, reference);
    }

    // Step 1: populate all synonyms
    for (const auto& clause : query_obj.clauses) {
        const auto data_source = DataSource{read_facade, curr_table};
        if (const auto such_that_clause = std::dynamic_pointer_cast<qps::SuchThatClause>(clause)) {
            const auto relationship = such_that_clause->rel_ref;
            evaluator = std::visit(such_that_clause_evaluator_selector(data_source, read_facade, false), relationship);
        } else if (const auto pattern_clause = std::dynamic_pointer_cast<qps::PatternClause>(clause)) {
            const auto syntactic_pattern = pattern_clause->syntactic_pattern;
            evaluator =
                std::visit(pattern_clause_evaluator_selector(data_source, read_facade, false), syntactic_pattern);
        } else if (const auto with_clause = std::dynamic_pointer_cast<qps::WithClause>(clause)) {
            evaluator =
                std::make_shared<WithEvaluator>(data_source, read_facade, with_clause->ref1, with_clause->ref2, false);
        }

        if (evaluator == nullptr) {
#ifdef DEBUG
            std::cerr << "Failed to create evaluator for clause: " << *clause << std::endl;
#endif
            auto empty_table = OutputTable{Table{}};
            return project_to_table(read_facade, empty_table, reference);
        }

        auto next_table = evaluator->evaluate();

        if (clause->is_negated_clause()) {
            if (is_empty(next_table)) {
                continue;
            }
            curr_table = subtract(std::move(curr_table), std::move(next_table), read_facade);
        } else {
            if (is_empty(next_table)) {
#ifdef DEBUG
                std::cerr << "Failed to evaluate clause: " << *clause << std::endl;
#endif
                return project_to_table(read_facade, next_table, reference);
            }
            curr_table = join(std::move(curr_table), std::move(next_table));
        }
        if (is_empty(curr_table)) {
#ifdef DEBUG
            std::cerr << "Conflict detected for clause: " << *clause << std::endl;
#endif
            return project_to_table(read_facade, curr_table, reference);
        }
    }

    return project_to_table(read_facade, curr_table, reference);
}

auto QueryEvaluator::evaluate(const qps::Query& query_obj) -> std::vector<std::string> {
    // Step 1: optimise query
    const auto optimised_queries = optimise(query_obj);
    if (has_contradiction(optimised_queries)) {
        auto table = OutputTable{Table{}};
        return project(read_facade, table, query_obj.reference);
    }

    // Step 2: evaluate optimised queries
    auto curr_table = OutputTable{UnitTable{}};
    for (const auto& query : optimised_queries) {
        auto next_table = evaluate_query(query);
        if (is_empty(next_table)) {
            return project(read_facade, next_table, query_obj.reference);
        }

        curr_table = join(std::move(curr_table), std::move(next_table));
        if (is_empty(curr_table)) {
            return project(read_facade, curr_table, query_obj.reference);
        }
    }

    // Step 2: project to relevant synonym
    return project(read_facade, curr_table, query_obj.reference);
}

} // namespace qps