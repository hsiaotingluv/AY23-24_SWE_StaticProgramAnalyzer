#pragma once

#include "qps/optimisers/optimiser.hpp"

namespace qps {
class DefaultOptimiser : public Optimiser {
    [[nodiscard]] auto optimise(const Query& query) const -> std::vector<Query> override {
        return {query};
    }
};
} // namespace qps
