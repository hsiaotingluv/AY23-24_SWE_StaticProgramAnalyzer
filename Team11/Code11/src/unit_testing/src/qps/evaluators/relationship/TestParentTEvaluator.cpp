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

TEST_CASE("Test Evaluator Parent*") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    const auto if_strs = std::vector<std::string>{"1", "2"};
    const auto while_strs = std::vector<std::string>{"4", "5"};
    const auto assign_strs = std::vector<std::string>{"3", "6", "7"};

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
    write_facade->add_parent("2", "3");
    write_facade->add_parent("4", "5");
    write_facade->add_parent("5", "6");
    write_facade->finalise_pkb();

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Parent*(s1, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ParentT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(if1, s2)") {
        const auto query = Query{
            std::make_shared<IfSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ParentT{std::make_shared<IfSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, if2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ParentT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<IfSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(while1, s2)") {
        const auto query = Query{
            std::make_shared<WhileSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ParentT{std::make_shared<WhileSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, while2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ParentT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<WhileSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"4"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, 3)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{std::make_shared<AnyStmtSynonym>("s1"), Integer{"3"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, 7)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{std::make_shared<AnyStmtSynonym>("s1"), Integer{"7"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that not Parent*(s1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}, true),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"3", "6", "7"});
    }

    SECTION("Evaluate - Select s2 such that Parent*(1, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(1, 3)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{Integer{"1"}, Integer{"3"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
    }

    SECTION("Evaluate - Select s1 such that not Parent*(1, 3)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{Integer{"1"}, Integer{"3"}}, true),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent*(1, 7)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{Integer{"1"}, Integer{"7"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that not Parent*(1, 7)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{Integer{"1"}, Integer{"7"}}, true),
            },
        };

        const auto output = evaluator.evaluate(query);
        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{Integer{"1"}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
    }

    SECTION("Evaluate - Select s2 such that Parent*(_, s2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{WildCard{}, std::make_shared<AnyStmtSynonym>("s2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(_, 2)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{WildCard{}, Integer{"2"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(_, 7)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{WildCard{}, Integer{"7"}}, false),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(_, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ParentT{WildCard{}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
    }
}
