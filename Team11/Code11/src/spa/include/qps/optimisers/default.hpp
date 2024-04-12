#pragma once

#include "qps/optimisers/optimiser.hpp"

#include "qps/optimisers/grouping.hpp"
#include "qps/optimisers/priority.hpp"
#include "qps/optimisers/redundancy.hpp"

#include <memory>
#include <vector>

namespace qps {
class DefaultOptimiser : public Optimiser {
    const std::array<std::shared_ptr<Optimiser>, 3> optimisers = {std::make_shared<RedundancyOptimiser>(),
                                                                  std::make_shared<GroupingOptimiser>(),
                                                                  std::make_shared<PriorityOptimiser>()};

    [[nodiscard]] auto optimise(const Query& query) const -> std::vector<Query> override {
        auto optimised_queries = std::vector<Query>{query};

        for (const auto& optimiser : optimisers) {
            auto new_optimised_queries = std::vector<Query>{};
            for (const auto& optimised_query : optimised_queries) {
                const auto new_optimised_query = optimiser->optimise(std::vector<Query>{optimised_query});
                new_optimised_queries.insert(new_optimised_queries.end(), new_optimised_query.begin(),
                                             new_optimised_query.end());
            }
            optimised_queries = new_optimised_queries;
        }
        return optimised_queries;
    }
};
} // namespace qps
