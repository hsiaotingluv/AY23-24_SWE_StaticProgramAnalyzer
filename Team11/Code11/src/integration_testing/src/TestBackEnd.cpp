#include "catch.hpp"

#include "PKB/Facades/ReadFacade.h"
#include "PKB/Facades/WriteFacade.h"
#include "PKB/PKB.h"

#include "common/statement_type.hpp"
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

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
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

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
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
        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1", "2"};
        REQUIRE(results.size() == 2);
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }
}

TEST_CASE("Test PKB and QPS - Statements") {
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
    writeFacade->addStatement("1", StatementType::Assign);
    writeFacade->addStatement("2", StatementType::If);
    writeFacade->addStatement("3", StatementType::While);
    writeFacade->addStatement("4", StatementType::Call);
    writeFacade->addStatement("5", StatementType::Read);
    writeFacade->addStatement("6", StatementType::Print);
    writeFacade->addStatement("7", StatementType::Raw);

    SECTION("Test Query - all assign") {
        const auto query = "assign a; Select a";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all if") {
        const auto query = "if i; Select i";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"2"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all while") {
        const auto query = "while w; Select w";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"3"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all call") {
        const auto query = "call c; Select c";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"4"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all read") {
        const auto query = "read r; Select r";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"5"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all print") {
        const auto query = "print p; Select p";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        REQUIRE(query_obj.clauses.empty());

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"6"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }
}

TEST_CASE("Test PKB and QPS - Modifies(stmt, var)") {
    auto pkb = std::make_shared<PKB>();
    auto readFacade = std::make_shared<ReadFacade>(pkb);
    auto writeFacade = std::make_shared<WriteFacade>(pkb);
    const auto qps_parser = qps::QueryProcessingSystemParser{};

    // Populate the PKB with some data
    writeFacade->addProcedure("procedure1");
    writeFacade->addStatement("1", StatementType::Assign);
    writeFacade->addStatement("2", StatementType::Call);
    writeFacade->addVariable("x");

    writeFacade->addStatementModifiesVar("1", "x");

    SECTION("Test Query - all variables modified by statement") {
        const auto query = R"(variable v; Select v such that Modifies(1, v))";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"x"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - all modifying assignments") {
        const auto query = R"(assign a; Select a such that Modifies(a, _))";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - two synonyms") {
        const auto query = R"(variable v; assign a; Select a such that Modifies(a, v))";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }

    SECTION("Test Query - modifies a particular synonym") {
        const auto query = R"(assign a; Select a such that Modifies(a, "x"))";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"1"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }

        const auto query2 = R"(assign a; Select a such that Modifies(a, "y"))";
        const auto maybe_query_obj2 = qps_parser.parse(query2);

        REQUIRE(maybe_query_obj2.has_value());
        const auto query_obj2 = maybe_query_obj2.value();

        const auto results2 = evaluator.evaluate(query_obj2);
        for (const auto& x : results2) {
            std::cout << x << std::endl;
        }
        REQUIRE(results2.empty());
    }

    SECTION("Test Query - impossible query") {
        const auto query = R"(variable v; Select v such that Modifies(2, _))";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        REQUIRE(results.empty());
    }

    SECTION("Test Query - get all query") {
        const auto query = R"(variable v; Select v such that Modifies(1, _))";
        const auto maybe_query_obj = qps_parser.parse(query);

        REQUIRE(maybe_query_obj.has_value());
        const auto query_obj = maybe_query_obj.value();

        auto evaluator = qps::Evaluator{readFacade};
        const auto results = evaluator.evaluate(query_obj);
        const auto expected = std::unordered_set<std::string>{"x"};
        REQUIRE(results.size() == expected.size());
        for (const auto& x : results) {
            REQUIRE(expected.find(x) != expected.end());
        }
    }
}