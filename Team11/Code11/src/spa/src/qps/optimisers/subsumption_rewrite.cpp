#include "qps/optimisers/subsumption_rewrite.hpp"
#include "qps/parser/entities/clause.hpp"

#include "qps/optimisers/subsumption_relation.tpp"
#include "qps/template_utils.hpp"
#include <memory>
#include <vector>

namespace qps {

template <typename T, typename U>
static auto subsumes(const T& relationship1, const U& relationship2) -> bool {
    return type_subsume(relationship1, relationship2) && args_subsume(relationship1, relationship2);
}

static auto subsumes(const Relationship& relationship1, const Relationship& relationship2) -> bool {
    return std::visit(overloaded{[](const auto& relationship1, const auto& relationship2) -> bool {
                          return subsumes(relationship1, relationship2);
                      }},
                      relationship1, relationship2);
}

auto SubsumptionRewriteOptimiser::optimise(const Query& query) const -> std::vector<Query> {
    auto clauses = query.clauses;
    const auto mid_it = std::partition(clauses.begin(), clauses.end(), [](const auto& clause) {
        return std::dynamic_pointer_cast<SuchThatClause>(clause) == nullptr;
    });

    if (std::distance(mid_it, clauses.end()) < 2) {
        // Less than 2 such that clauses, no optimisation possible
        return {query};
    }

    auto discarded_clauses = std::unordered_set<std::shared_ptr<Clause>>{};
    for (auto it1 = mid_it; it1 != clauses.end() - 1; ++it1) {
        for (auto it2 = it1 + 1; it2 != clauses.end(); ++it2) {
            const auto& clause1 = std::dynamic_pointer_cast<SuchThatClause>(*it1);
            const auto& clause2 = std::dynamic_pointer_cast<SuchThatClause>(*it2);

            if (clause1->is_negated_clause() != clause2->is_negated_clause()) {
                continue;
            }

            const auto& relationship1 = clause1->rel_ref;
            const auto& relationship2 = clause2->rel_ref;

            const auto does_subsume = !clause1->is_negated_clause() ? subsumes(relationship1, relationship2)
                                                                    : subsumes(relationship2, relationship1);
            if (does_subsume) {
                discarded_clauses.insert(clause2);
            }
        }
    }

    const auto end_it = std::partition(mid_it, clauses.end(), [&discarded_clauses](const auto& clause) {
        return discarded_clauses.find(clause) == discarded_clauses.end();
    });
    clauses.erase(end_it, std::end(clauses));
    return {Query{query.reference, clauses}};
}
} // namespace qps