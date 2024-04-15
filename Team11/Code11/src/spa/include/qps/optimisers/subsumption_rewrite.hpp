#pragma once

#include "qps/optimisers/optimiser.hpp"

namespace qps {
class SubsumptionRewriteOptimiser : public Optimiser {
    [[nodiscard]] auto optimise(const Query& query) const -> std::vector<Query> override;
};

} // namespace qps