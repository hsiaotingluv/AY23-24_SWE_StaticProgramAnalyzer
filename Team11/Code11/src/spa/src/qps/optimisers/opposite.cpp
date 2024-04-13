#include "qps/optimisers/opposite.hpp"
#include "qps/parser/entities/clause.hpp"

namespace qps {
auto OppositeOptimiser::optimise(const Query& query) const -> std::vector<Query> {
    if (query.clauses.size() < 2) {
        return {query};
    }

    for (auto it1 = query.clauses.begin(); it1 != query.clauses.end() - 1; ++it1) {
        for (auto it2 = it1 + 1; it2 != query.clauses.end(); ++it2) {
            if ((*it1)->is_equal_modulo_negation(*(*it2)) &&
                (*it1)->is_negated_clause() != (*it2)->is_negated_clause()) {
                return {Query{query.reference, {std::make_shared<ContradictionClause>()}}};
            }
        }
    }

    return {query};
}
} // namespace qps