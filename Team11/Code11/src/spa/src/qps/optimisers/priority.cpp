#include "qps/optimisers/priority.hpp"
#include <algorithm>
#include <vector>

namespace qps {

auto PriorityOptimiser::optimise(const Query& query) const -> std::vector<Query> {
    auto clauses = query.clauses;
    std::stable_partition(clauses.begin(), clauses.end(), [](const auto& clause) {
        return !clause->is_negated_clause();
    });
    return {Query{query.reference, std::move(clauses)}};
}
} // namespace qps