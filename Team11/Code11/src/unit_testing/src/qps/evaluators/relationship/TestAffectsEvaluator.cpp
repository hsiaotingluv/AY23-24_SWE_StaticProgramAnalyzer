#include "test_evaluator.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb_manager.h"

#include "qps/evaluators/query_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <memory>
#include <vector>

using namespace qps;

using namespace pkb;

TEST_CASE("Test Evaluator Affects") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    const auto assign_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }

    write_facade->add_next("1", "2");
    write_facade->add_next("2", "3");
    write_facade->add_next("3", "4");
    write_facade->add_next("4", "5");

    // Add variables and their modifications
    write_facade->add_variable("x");
    write_facade->add_statement_modify_var("1", "x");
    write_facade->add_statement_modify_var("2", "x");
    write_facade->add_statement_modify_var("3", "z");
    write_facade->add_statement_modify_var("4", "z");
    write_facade->add_statement_modify_var("5", "x");

    // Add uses relationships
    write_facade->add_statement_use_var("2", "x");
    write_facade->add_statement_use_var("2", "y");
    write_facade->add_statement_use_var("4", "z");
    write_facade->add_statement_use_var("4", "x");
    write_facade->add_statement_use_var("5", "x");
    write_facade->add_statement_use_var("5", "y");
    write_facade->add_statement_use_var("5", "z");

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Affects (s1, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Affects{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4"});
    }

    SECTION("Evaluate - Select s1 such that Affects (s1, 5)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{std::make_shared<AnyStmtSynonym>("s1"), Integer{"5"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "4"});
    }

    SECTION("Evaluate - Select s1 such that Affects (s1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4"});
    }

    // TODO: Fail - program failure
    SECTION("Evaluate - Select s1 such that Affects (1, s1)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s1")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2"});
    }

    SECTION("Evaluate - Select s1 such that Affects (2, 4)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"2"}, Integer{"4"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Affects (1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"1"}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Affects (_, s1)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, std::make_shared<AnyStmtSynonym>("s1")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Affects (_, 4)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, Integer{"4"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Affects (_, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Affects (5, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"5"}, WildCard{}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Affects (_, 3)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, Integer{"3"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }
}

TEST_CASE("Test Evaluator Affects with Cycle") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    const auto assign_strs = std::vector<std::string>{"1", "3", "5", "6"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }
    write_facade->add_statement("2", StatementType::While);
    write_facade->add_statement("4", StatementType::If);

    write_facade->add_next("1", "2");
    write_facade->add_next("2", "3");
    write_facade->add_next("3", "2");
    write_facade->add_next("2", "4");
    write_facade->add_next("4", "5");
    write_facade->add_next("4", "6");

    // Add variables and their modifications
    write_facade->add_variable("i");
    write_facade->add_statement_modify_var("1", "i");
    write_facade->add_statement_modify_var("3", "i");
    write_facade->add_statement_modify_var("5", "i");
    write_facade->add_statement_modify_var("6", "i");

    // Add uses relationships
    write_facade->add_statement_use_var("1", "i");
    write_facade->add_statement_use_var("2", "i");
    write_facade->add_statement_use_var("3", "i");
    write_facade->add_statement_use_var("4", "i");
    write_facade->add_statement_use_var("5", "i");
    write_facade->add_statement_use_var("6", "i");

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Affects (s1, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Affects{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "3"});
    }

    SECTION("Evaluate - Select s1 such that Affects (s1, 5)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{std::make_shared<AnyStmtSynonym>("s1"), Integer{"5"}}, false),
            },
        };

        auto temp = evaluator.evaluate(query);

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "3"});
    }

    SECTION("Evaluate - Select s1 such that Affects (s1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "3"});
    }

    SECTION("Evaluate - Select s1 such that Affects (1, s1)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s1")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"3", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Affects (3, 3)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"3"}, Integer{"3"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Affects (1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"1"}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Affects (_, s1)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, std::make_shared<AnyStmtSynonym>("s1")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"3", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Affects (_, 5)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, Integer{"5"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Affects (_, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Affects (2, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"2"}, WildCard{}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Affects (_, 4)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, Integer{"4"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }
}

TEST_CASE("Test Evaluator Affects with Nested Cycle") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    const auto assign_strs = std::vector<std::string>{"1", "2", "4", "6", "7", "9", "11", "12", "13", "14"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }
    write_facade->add_statement("3", StatementType::While);
    write_facade->add_statement("8", StatementType::While);
    write_facade->add_statement("5", StatementType::If);
    write_facade->add_statement("10", StatementType::If);

    write_facade->add_next("1", "2");
    write_facade->add_next("2", "3");
    write_facade->add_next("3", "4");
    write_facade->add_next("4", "5");
    write_facade->add_next("5", "6");
    write_facade->add_next("5", "7");
    write_facade->add_next("6", "8");
    write_facade->add_next("7", "8");
    write_facade->add_next("8", "9");
    write_facade->add_next("9", "10");
    write_facade->add_next("10", "11");
    write_facade->add_next("10", "12");
    write_facade->add_next("11", "8");
    write_facade->add_next("12", "8");
    write_facade->add_next("8", "13");
    write_facade->add_next("13", "3");
    write_facade->add_next("3", "14");

    // Add variables and their modifications
    write_facade->add_variable("x");
    write_facade->add_statement_modify_var("1", "x");
    write_facade->add_statement_modify_var("4", "x");
    write_facade->add_statement_modify_var("11", "x");
    write_facade->add_statement_modify_var("12", "x");
    write_facade->add_statement_modify_var("13", "x");

    write_facade->add_variable("y");
    write_facade->add_statement_modify_var("2", "y");
    write_facade->add_statement_modify_var("6", "y");
    write_facade->add_statement_modify_var("7", "y");
    write_facade->add_statement_modify_var("9", "y");
    write_facade->add_statement_modify_var("14", "y");

    // Add uses relationships
    write_facade->add_statement_use_var("4", "x");
    write_facade->add_statement_use_var("7", "x");
    write_facade->add_statement_use_var("11", "x");
    write_facade->add_statement_use_var("14", "x");

    write_facade->add_statement_use_var("6", "y");
    write_facade->add_statement_use_var("9", "y");
    write_facade->add_statement_use_var("12", "y");
    write_facade->add_statement_use_var("13", "y");

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Affects (s1, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Affects{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query),
                      std::vector<std::string>{"1", "2", "4", "6", "7", "9", "11", "12", "13"});
    }

    SECTION("Evaluate - Select s1 such that Affects (s1, 9)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{std::make_shared<AnyStmtSynonym>("s1"), Integer{"9"}}, false),
            },
        };

        auto temp = evaluator.evaluate(query);

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"6", "7", "9"});
    }

    SECTION("Evaluate - Select s1 such that Affects (s1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query),
                      std::vector<std::string>{"1", "2", "4", "6", "7", "9", "11", "12", "13"});
    }

    SECTION("Evaluate - Select s1 such that Affects (1, s1)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s1")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"4", "14"});
    }

    SECTION("Evaluate - Select s1 such that Affects (9, 9)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"9"}, Integer{"9"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7", "8", "9",
                                                                          "10", "11", "12", "13", "14"});
    }

    SECTION("Evaluate - Select s1 such that Affects (1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"1"}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7", "8", "9",
                                                                          "10", "11", "12", "13", "14"});
    }

    SECTION("Evaluate - Select s1 such that Affects (_, s1)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, std::make_shared<AnyStmtSynonym>("s1")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"4", "6", "7", "9", "11", "12", "13", "14"});
    }

    SECTION("Evaluate - Select s1 such that Affects (_, 6)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, Integer{"6"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7", "8", "9",
                                                                          "10", "11", "12", "13", "14"});
    }

    SECTION("Evaluate - Select s1 such that Affects (_, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7", "8", "9",
                                                                          "10", "11", "12", "13", "14"});
    }

    SECTION("Evaluate - Select s1 such that Affects (3, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{Integer{"3"}, WildCard{}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Affects (_, 5)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Affects{WildCard{}, Integer{"5"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }
}