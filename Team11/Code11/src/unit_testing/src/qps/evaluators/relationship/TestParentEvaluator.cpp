#include "test_evaluator.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb.h"

#include "qps/evaluators/query_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <memory>
#include <vector>

using namespace qps;
using namespace pkb;

TEST_CASE("Test Evaluator Parent") {
    const auto& [read_facade, write_facade] = PKB::create_facades();

    // Populate PKB
    const auto if_strs = std::vector<std::string>{"1"};
    const auto while_strs = std::vector<std::string>{"3"};
    const auto assign_strs = std::vector<std::string>{"2", "4", "5"};

    for (const auto& x : if_strs) {
        write_facade->add_statement(x, StatementType::If);
    }
    for (const auto& x : while_strs) {
        write_facade->add_statement(x, StatementType::While);
    }
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }

    write_facade->add_parent("1", "2");
    write_facade->add_parent("3", "4");

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Parent(s1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Parent{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "3"});
    }

    SECTION("Evaluate - Select s1 such that Parent(if1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<IfSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<IfSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Parent{std::make_shared<IfSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select s1 such that Parent(s1, if2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<IfSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Parent{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<IfSynonym>("s2")}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(while1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<WhileSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<WhileSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Parent{std::make_shared<WhileSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"3"});
    }

    SECTION("Evaluate - Select s1 such that Parent(s1, while2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<WhileSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Parent{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<WhileSynonym>("s2")}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(s1, 4)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{std::make_shared<AnyStmtSynonym>("s1"), Integer{"4"}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"3"});
    }

    SECTION("Evaluate - Select s1 such that Parent(s1, 5)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{std::make_shared<AnyStmtSynonym>("s1"), Integer{"5"}}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(s1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "3"});
    }

    SECTION("Evaluate - Select s2 such that Parent(3, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{Integer{"3"}, std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"4"});
    }

    SECTION("Evaluate - Select s1 such that Parent(3, 4)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{Integer{"3"}, Integer{"4"}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Parent(3, 5)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{Integer{"3"}, Integer{"5"}}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(3, _)") {
        const auto query = Query{
            Synonyms{std::make_shared<AnyStmtSynonym>("s1")},
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{Integer{"3"}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select s2 such that Parent(_, s2)") {
        const auto query = Query{
            Synonyms{std::make_shared<AnyStmtSynonym>("s2")},
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{WildCard{}, std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "4"});
    }

    SECTION("Evaluate - Select s1 such that Parent(_, 2)") {
        const auto query = Query{
            Synonyms{std::make_shared<AnyStmtSynonym>("s1")},
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{WildCard{}, Integer{"2"}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Parent(_, 3)") {
        const auto query = Query{
            Synonyms{std::make_shared<AnyStmtSynonym>("s1")},
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{WildCard{}, Integer{"3"}}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(_, _)") {
        const auto query = Query{
            Synonyms{std::make_shared<AnyStmtSynonym>("s1")},
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Parent{WildCard{}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }
}
