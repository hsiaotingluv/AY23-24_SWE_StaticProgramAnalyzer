#pragma once

#include "qps/optimisers/optimiser.hpp"

namespace qps {

class GroupingOptimiser : public Optimiser {
    [[nodiscard]] auto optimise(const Query& query) const -> std::vector<Query> override;
};

} // namespace qps