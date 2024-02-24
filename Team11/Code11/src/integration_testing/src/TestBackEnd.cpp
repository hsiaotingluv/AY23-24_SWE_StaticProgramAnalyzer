#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb.h"

#include "common/statement_type.hpp"
#include "qps/evaluators/query_evaluator.hpp"
#include "qps/qps.hpp"

#include <memory>
#include <unordered_set>

TEST_CASE("Test pkb and QPS - Entities") {
    auto [read_facade, write_facade] = PKB::create_facades();
    const auto qps_parser = qps::QueryProcessingSystem{};

    // Populate the pkb with some data
    write_facade->add_procedure("procedure1");
    write_facade->add_variable("x");
    write_facade->add_variable("y");
    write_facade->add_variable("z");
    write_facade->add_constant("1");
    write_facade->add_constant("2");

    SECTION("Test Query - all variables") {
        const auto query = "variable v; Select v";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"x", "y", "z"};
        REQUIRE(results.size() == 3);
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all procedures") {
        const auto query = "procedure p; Select p";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"procedure1"};
        REQUIRE(results.size() == 1);
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all constants") {
        const auto query = "constant c; Select c";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        // Simple evaluator:
        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1", "2"};
        REQUIRE(results.size() == 2);
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }
}

TEST_CASE("Test pkb and QPS - Statements") {
    auto [read_facade, write_facade] = PKB::create_facades();
    const auto qps_parser = qps::QueryProcessingSystem{};

    // Populate the pkb with some data
    write_facade->add_procedure("procedure1");
    write_facade->add_variable("x");
    write_facade->add_variable("y");
    write_facade->add_variable("z");
    write_facade->add_constant("1");
    write_facade->add_constant("2");
    write_facade->add_statement("1", StatementType::Assign);
    write_facade->add_statement("2", StatementType::If);
    write_facade->add_statement("3", StatementType::While);
    write_facade->add_statement("4", StatementType::Call);
    write_facade->add_statement("5", StatementType::Read);
    write_facade->add_statement("6", StatementType::Print);

    SECTION("Test Query - all assign") {
        const auto query = "assign a; Select a";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all if") {
        const auto query = "if i; Select i";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"2"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all while") {
        const auto query = "while w; Select w";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"3"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all call") {
        const auto query = "call c; Select c";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"4"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all read") {
        const auto query = "read r; Select r";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"5"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all print") {
        const auto query = "print p; Select p";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));
        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"6"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }
}

TEST_CASE("Test pkb and QPS - Modifies(stmt, var)") {
    auto [read_facade, write_facade] = PKB::create_facades();

    const auto qps_parser = qps::QueryProcessingSystem{};

    // Populate the pkb with some data
    write_facade->add_procedure("procedure1");
    write_facade->add_statement("1", StatementType::Assign);
    write_facade->add_statement("2", StatementType::Call);
    write_facade->add_variable("x");

    write_facade->add_statement_modifies_var("1", "x");

    SECTION("Test Query - all variables modified by statement") {
        const auto query = R"(variable v; Select v such that Modifies(1, v))";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"x"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all modifying assignments") {
        const auto query = R"(assign a; Select a such that Modifies(a, _))";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - two synonyms") {
        const auto query = R"(variable v; assign a; Select a such that Modifies(a, v))";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - modifies a particular synonym") {
        const auto query = R"(assign a; Select a such that Modifies(a, "x"))";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }

        const auto query2 = R"(assign a; Select a such that Modifies(a, "y"))";
        const auto maybe_query_obj2 = qps::to_query(qps_parser.parse(query2));

        REQUIRE(maybe_query_obj2.has_value());
        const auto query_obj2 = maybe_query_obj2.value();

        const auto results2 = evaluator.evaluate(query_obj2);
        REQUIRE(results2.empty());
    }

    SECTION("Test Query - impossible query") {
        const auto query = R"(variable v; Select v such that Modifies(2, _))";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        REQUIRE(results.empty());
    }

    SECTION("Test Query - get all query") {
        const auto query = R"(variable v; Select v such that Modifies(1, _))";
        const auto maybe_query_obj = qps::to_query(qps_parser.parse(query));

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::QueryEvaluator{read_facade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"x"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }
}