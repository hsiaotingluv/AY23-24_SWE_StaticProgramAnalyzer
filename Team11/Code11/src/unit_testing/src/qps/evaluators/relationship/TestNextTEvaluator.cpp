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

TEST_CASE("Test Evaluator Next*") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    const auto assign_strs = std::vector<std::string>{"1", "2", "3"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }
    write_facade->add_next("1", "2");
    write_facade->add_next("2", "3");

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Next* (s1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    NextT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Next* (s1, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{std::make_shared<AnyStmtSynonym>("s1"), Integer{"3"}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Next* (s1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Next* (1, s1)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s1")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Next* (1, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{Integer{"1"}, Integer{"3"}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Next* (1, 5)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{Integer{"1"}, Integer{"5"}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Next* (1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{Integer{"1"}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Next* (5, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{Integer{"5"}, WildCard{}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Next* (10, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{Integer{"10"}, WildCard{}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Next* (_, s1)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{WildCard{}, std::make_shared<AnyStmtSynonym>("s1")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Next* (_, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{WildCard{}, Integer{"3"}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Next* (_, 1)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{WildCard{}, Integer{"1"}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Next* (_, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(NextT{WildCard{}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }
}
