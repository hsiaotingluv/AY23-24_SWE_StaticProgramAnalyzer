#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/results_table.hpp"

namespace qps {
class ClauseEvaluator {
protected:
    std::shared_ptr<ReadFacade> read_facade;

public:
    virtual ~ClauseEvaluator() = default;

    explicit ClauseEvaluator(std::shared_ptr<ReadFacade> read_facade) : read_facade(std::move(read_facade)){}

    virtual auto evaluate() -> std::optional<Table>;
};
}