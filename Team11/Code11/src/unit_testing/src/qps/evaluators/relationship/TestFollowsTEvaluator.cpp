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

TEST_CASE("Test Evaluator Follows*") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    const auto assign_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }
    write_facade->add_follows("1", "2");
    write_facade->add_follows("2", "3");
    write_facade->finalise_pkb();

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Follows* (s1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    FollowsT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (s1, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{std::make_shared<AnyStmtSynonym>("s1"), Integer{"3"}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (s1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (1, s1)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s1")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (1, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{Integer{"1"}, Integer{"3"}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Follows* (1, 5)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{Integer{"1"}, Integer{"5"}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Follows* (1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{Integer{"1"}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Follows* (5, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{Integer{"5"}, WildCard{}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Follows* (10, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{Integer{"10"}, WildCard{}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Follows* (_, s1)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{WildCard{}, std::make_shared<AnyStmtSynonym>("s1")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (_, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{WildCard{}, Integer{"3"}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Follows* (_, 1)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{WildCard{}, Integer{"1"}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Follows* (_, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(FollowsT{WildCard{}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }
}
