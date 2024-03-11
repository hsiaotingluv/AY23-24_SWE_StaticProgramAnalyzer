#pragma once
#include "catch.hpp"

#include "common/statement_type.hpp"
#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb.h"

#include "qps/evaluators/query_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <algorithm>
#include <memory>
#include <vector>

using namespace qps;

template<typename T>
auto require_equal(const std::vector<T>& actual, const std::vector<T>& expected) -> void {
    REQUIRE(actual.size() == expected.size());
    for (const auto& value : actual) {
        auto it = std::find(expected.begin(), expected.end(), value);
        REQUIRE(it != expected.end());
    }
}