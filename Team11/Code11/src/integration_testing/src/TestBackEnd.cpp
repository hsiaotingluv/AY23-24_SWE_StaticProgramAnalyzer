#include "catch.hpp"

#include "PKB/Facades/ReadFacade.h"
#include "PKB/Facades/WriteFacade.h"
#include "PKB/PKB.h"

#include "qps/evaluators/simple_evaluator.hpp"
#include "qps/parser/parser.hpp"

#include <memory>
#include <optional>
#include <unordered_set>

TEST_CASE("Test PKB and QPS - Entities") {
    auto pkb = std::make_shared<PKB>();
    auto readFacade = std::make_shared<ReadFacade>(pkb);
    auto writeFacade = std::make_shared<WriteFacade>(pkb);
    const auto qps_parser = qps::QueryProcessingSystemParser{};

    // Populate the PKB with some data
    writeFacade->addProcedure("procedure1");
    writeFacade->addVariable("x");
    writeFacade->addVariable("y");
    writeFacade->addVariable("z");
    writeFacade->addConstant("1");
    writeFacade->addConstant("2");

    SECTION("Test Query - all variables") {
        const auto query = "variable v; Select v";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        const auto results = qps::stub::evaluate(query_obj, readFacade);
        const auto expected = std::unordered_set<std::string>{"x", "y", "z"};
        REQUIRE(results.size() == 3);
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all procedures") {
        const auto query = "procedure p; Select p";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        const auto results = qps::stub::evaluate(query_obj, readFacade);
        const auto expected = std::unordered_set<std::string>{"procedure1"};
        REQUIRE(results.size() == 1);
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all constants") {
        const auto query = "constant c; Select c";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        // Simple evaluator:
        const auto results = qps::stub::evaluate(query_obj, readFacade);
        const auto expected = std::unordered_set<std::string>{"1", "2"};
        REQUIRE(results.size() == 2);
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }
}