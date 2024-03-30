#include "qps/optimisers/redundancy.hpp"
#include "qps/parser/entities/clause.hpp"
#include <memory>
#include <vector>

namespace qps {

auto RedundancyOptimiser::optimise(const Query& query) const -> std::vector<Query> {
    const auto& select = query.reference;

    const auto& clauses = query.clauses;

    auto redundant_removed = std::vector<std::shared_ptr<Clause>>{};
    for (const auto& clause : clauses) {
        auto is_seen_before = false;
        for (const auto& seen_before : redundant_removed) {
            if (clause == seen_before) {
                is_seen_before = true;
                break;
            }
        }

        if (!is_seen_before) {
            redundant_removed.push_back(std::move(clause));
        }
    }

    return {Query{select, redundant_removed}};
}
} // namespace qps