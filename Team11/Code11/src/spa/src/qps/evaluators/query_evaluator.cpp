#include "qps/evaluators/query_evaluator.hpp"
#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator_selector.hpp"
#include "qps/evaluators/results_table.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <variant>
#include <vector>

namespace qps {

auto build_table(const std::vector<Elem>& elems, const std::shared_ptr<pkb::ReadFacade>& read_facade) -> OutputTable {
    auto synonyms = Synonyms{};
    synonyms.reserve(elems.size());
    std::for_each(elems.begin(), elems.end(), [&synonyms](const auto& elem) {
        return std::visit(overloaded{[&synonyms](const std::shared_ptr<Synonym>& synonym) {
                              synonyms.push_back(synonym);
                          }},
                          elem);
    });

    auto table = Table{};
    for (const auto& synonym : synonyms) {
        auto curr_table = Table{{synonym}};
        for (const auto& result : synonym->scan(read_facade)) {
            curr_table.add_row({result});
        }
        table = std::get<Table>(detail::cross_join(table, curr_table));
    }
    return table;
}

auto build_table(const Reference& reference, const std::shared_ptr<pkb::ReadFacade>& read_facade) -> OutputTable {
    return std::visit(overloaded{[](const BooleanReference&) -> OutputTable {
                                     return UnitTable{};
                                 },
                                 [&read_facade](const std::vector<Elem>& elems) -> OutputTable {
                                     return build_table(elems, read_facade);
                                 }},
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
        } else if (const auto pattern_clause = std::dynamic_pointer_cast<qps::PatternClause>(clause)) {
            const auto syntactic_pattern = pattern_clause->syntactic_pattern;
            evaluator = std::visit(clause_evaluator_selector(read_facade), syntactic_pattern);
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