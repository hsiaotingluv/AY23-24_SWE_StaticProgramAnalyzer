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

TEST_CASE("Test Evaluator Follows") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    const auto assign_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }
    write_facade->add_follows("1", "2");
    write_facade->add_follows("2", "3");

    auto evaluator = QueryEvaluator{read_facade};
    SECTION("Evaluate - Select s1 such that Follows (s1, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Follows{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s2 such that Follows (1, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2"});
    }

    SECTION("Evaluate - Select s2 such that Follows (s2, 2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<AnyStmtSynonym>("s2"), Integer{"2"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select s2 such that Follows (s2, 10)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<AnyStmtSynonym>("s2"), Integer{"10"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s2 such that Follows (1, 2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{"1"}, Integer{"2"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s2 such that Follows (1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{"1"}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s2 such that Follows (_, 3)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{WildCard{}, Integer{"3"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s2 such that Follows (_, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{WildCard{}, std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s2 such that Follows (_, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{WildCard{}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Follows (s1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s2 such that Follows (1, 3)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{"1"}, Integer{"3"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s2 such that Follows (2, 10)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{"2"}, Integer{"10"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s2 such that Follows (2, 2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{"2"}, Integer{"2"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s2 such that Follows (s2, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Follows{std::make_shared<AnyStmtSynonym>("s2"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select w such that Follows (w, s2)") {
        const auto query = Query{
            std::make_shared<WhileSynonym>("w"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Follows{std::make_shared<WhileSynonym>("w"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select w such that Follows (w1, w2)") {
        const auto query = Query{
            std::make_shared<WhileSynonym>("w1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Follows{std::make_shared<WhileSynonym>("w1"), std::make_shared<WhileSynonym>("w2")}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }
}
