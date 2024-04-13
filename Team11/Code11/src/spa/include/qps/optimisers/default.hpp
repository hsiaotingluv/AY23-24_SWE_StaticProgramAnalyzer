#pragma once

#include "qps/optimisers/optimiser.hpp"

#include "qps/optimisers/grouping.hpp"
#include "qps/optimisers/opposite.hpp"
#include "qps/optimisers/priority.hpp"
#include "qps/optimisers/redundancy.hpp"
#include "qps/optimisers/subsumption_rewrite.hpp"

#include <memory>
#include <vector>

namespace qps {
static auto has_contradiction(const std::vector<Query>& queries) -> bool {
    if (queries.size() != 1) {
        return false;
    }

    if (queries.front().clauses.size() == 1 &&
        std::dynamic_pointer_cast<ContradictionClause>(queries.front().clauses.front()) != nullptr) {
        return true;
    }

    return false;
}

class DefaultOptimiser : public Optimiser {
    const std::array<std::shared_ptr<Optimiser>, 5> optimisers = {
        std::make_shared<RedundancyOptimiser>(), std::make_shared<OppositeOptimiser>(),
        std::make_shared<SubsumptionRewriteOptimiser>(), std::make_shared<GroupingOptimiser>(),
        std::make_shared<PriorityOptimiser>()};

    [[nodiscard]] auto optimise(const Query& query) const -> std::vector<Query> override {
        auto optimised_queries = std::vector<Query>{query};

        for (const auto& optimiser : optimisers) {
            auto new_optimised_queries = std::vector<Query>{};
            for (const auto& optimised_query : optimised_queries) {
                const auto new_optimised_query = optimiser->optimise(std::vector<Query>{optimised_query});
                if (has_contradiction(new_optimised_query)) {
                    return new_optimised_query;
                }

                new_optimised_queries.insert(new_optimised_queries.end(), new_optimised_query.begin(),
                                             new_optimised_query.end());
            }
            optimised_queries = new_optimised_queries;
        }
        return optimised_queries;
    }
};
} // namespace qps
