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

TEST_CASE("Test Evaluator Next") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    /*
     * 1 if (x)
     * 2 { <call> }
     * 3 } else { <print> }
     * 4 while (x)
     * 5 { <assign> }
     * 6 <read>
     */
    write_facade->add_statement("1", StatementType::If);
    write_facade->add_statement("2", StatementType::Call);
    write_facade->add_statement("3", StatementType::Print);
    write_facade->add_statement("4", StatementType::While);
    write_facade->add_statement("5", StatementType::Assign);
    write_facade->add_statement("6", StatementType::Read);

    write_facade->add_next("1", "2");
    write_facade->add_next("1", "3");
    write_facade->add_next("2", "4");
    write_facade->add_next("3", "4");
    write_facade->add_next("4", "5");
    write_facade->add_next("5", "4");
    write_facade->add_next("4", "6");

    write_facade->finalise_pkb();

    auto evaluator = QueryEvaluator{read_facade};
    SECTION("Evaluate - Select s1 such that Next (s1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Next{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select if1 such that Next (if1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<IfSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<IfSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Next{std::make_shared<IfSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select s2 such that Next (if1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<IfSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Next{std::make_shared<IfSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Next (s1, while2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<WhileSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Next{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<WhileSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3", "5"});
    }

    SECTION("Evaluate - Select while2 such that Next (s1, while2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<WhileSynonym>("s2"),
            },
            std::make_shared<WhileSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Next{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<WhileSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"4"});
    }

    SECTION("Evaluate - Select s2 such that Next (1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Next (s1, 4)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{std::make_shared<AnyStmtSynonym>("s1"), Integer{"4"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3", "5"});
    }

    SECTION("Evaluate - Select s1 such that Next (s1, 1)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{std::make_shared<AnyStmtSynonym>("s1"), Integer{"1"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s such that Next (1, 2) - returns all statements if true") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s"),
            },
            std::make_shared<AnyStmtSynonym>("s"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{Integer{"1"}, Integer{"2"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s such that Next (1, 6) - returns empty if false, should not be transitive") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s"),
            },
            std::make_shared<AnyStmtSynonym>("s"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{Integer{"1"}, Integer{"6"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s such that Next (1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s"),
            },
            std::make_shared<AnyStmtSynonym>("s"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{Integer{"1"}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s such that Next (_, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s"),
            },
            std::make_shared<AnyStmtSynonym>("s"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{WildCard{}, Integer{"3"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s2 such that Next (_, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{WildCard{}, std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s such that Next (_, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s"),
            },
            std::make_shared<AnyStmtSynonym>("s"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{WildCard{}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Next (s1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select s such that Next (2, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s"),
            },
            std::make_shared<AnyStmtSynonym>("s"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{Integer{"2"}, Integer{"3"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s such that Next (1, 4) - should not be transitive") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s"),
            },
            std::make_shared<AnyStmtSynonym>("s"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{Integer{"1"}, Integer{"4"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s2 such that Next (4, 4) - should not be transitive") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Next{Integer{"4"}, Integer{"4"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s such that Next (s, s) - should not be transitive") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s"),
            },
            std::make_shared<AnyStmtSynonym>("s"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Next{std::make_shared<AnyStmtSynonym>("s"), std::make_shared<AnyStmtSynonym>("s")}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }
}
