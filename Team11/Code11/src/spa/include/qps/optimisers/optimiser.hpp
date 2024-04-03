#pragma once

#include "qps/parser/analysers/semantic_analyser.hpp"
#include <vector>

namespace qps {
class Optimiser {
  private:
    [[nodiscard]] virtual auto optimise(const Query& query) const -> std::vector<Query> = 0;

  public:
    virtual ~Optimiser() = default;

    [[nodiscard]] auto optimise(const std::vector<Query>& queries) const -> std::vector<Query> {
        std::vector<Query> optimised_queries;
        for (const auto& query : queries) {
            auto optimised_query = optimise(query);
            optimised_queries.insert(optimised_queries.end(), optimised_query.begin(), optimised_query.end());
        }
        return optimised_queries;
    }
};

} // namespace qps