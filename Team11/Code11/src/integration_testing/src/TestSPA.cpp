#include "catch.hpp"

#include "sp/main.hpp"

#include "pkb/pkb_manager.h"

#include "qps/evaluators/query_evaluator.hpp"
#include "qps/parser.hpp"

#include <unordered_set>

using namespace pkb;

template <typename T>
void require_overlap(const T& actual, const std::unordered_set<std::string>& expected) {
    REQUIRE(actual.size() == expected.size());
    for (const auto& x : actual) {
        REQUIRE(expected.find(x) != expected.end());
    }
}

static auto input_generator = []() -> std::string {
    return R"(procedure main {
            flag = 0;
            call computeCentroid;
            call printResults;
        }

        procedure readPoint {
            read x;
            read y;
        }

        procedure printResults {
            print flag;
            print cenX;
            print cenY;
            print normSq;
        }

        procedure computeCentroid {
            count = 0;
            cenX = 0;
            cenY = 0;
            call readPoint;
            while ((x != 0) && (y != 0)) {
                count = count + 1;
                cenX = cenX + x;
                cenY = cenY + y;
                call readPoint;
            }
            if (count == 0) then {
                flag = 1;
            } else {
                cenX = cenX / count;
                cenY = cenY / count;
            }
            normSq = cenX * cenX + cenY * cenY;
        })";
};

TEST_CASE("Test SPA - Entities") {
    auto [read_facade, write_facade] = PkbManager::create_facades();

    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);

    const auto qps_parser = qps::DefaultParser{};

    SECTION("Test SP - complex program Code 4 - success") {
        auto input = input_generator();

        // Stage 1: Parse input and extract the entities
        auto ast = sp->process(input);

        const auto expected_constants = std::unordered_set<std::string>{"0", "1"};
        const auto expected_variables =
            std::unordered_set<std::string>{"flag", "x", "y", "count", "cenX", "cenY", "normSq"};
        const auto expected_procedures =
            std::unordered_set<std::string>{"main", "readPoint", "printResults", "computeCentroid"};

        // Stage 2: Answer queries
        SECTION("Test Query - all variables") {
            const auto query = "variable v; Select v";
            const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

            REQUIRE(maybe_query_obj.has_value());
            const auto query_obj = maybe_query_obj.value();

            REQUIRE(query_obj.clauses.empty());

            auto evaluator = qps::QueryEvaluator{read_facade};
            const auto results = evaluator.evaluate(query_obj);
            require_overlap(results, expected_variables);
        }

        SECTION("Test Query - all procedures") {
            const auto query = "procedure p; Select p";
            const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

            REQUIRE(maybe_query_obj.has_value());
            const auto query_obj = maybe_query_obj.value();

            REQUIRE(query_obj.clauses.empty());

            auto evaluator = qps::QueryEvaluator{read_facade};
            const auto results = evaluator.evaluate(query_obj);
            require_overlap(results, expected_procedures);
        }

        SECTION("Test Query - all constants") {
            const auto query = "constant c; Select c";
            const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

            REQUIRE(maybe_query_obj.has_value());
            const auto query_obj = maybe_query_obj.value();

            REQUIRE(query_obj.clauses.empty());

            auto evaluator = qps::QueryEvaluator{read_facade};
            const auto results = evaluator.evaluate(query_obj);
            require_overlap(results, expected_constants);
        }
    }
}
