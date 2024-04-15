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

TEST_CASE("Test Evaluator ModifiesS") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    write_facade->add_statement("1", StatementType::Assign);
    write_facade->add_statement("2", StatementType::Read);
    write_facade->add_statement("3", StatementType::If);
    write_facade->add_statement("4", StatementType::While);
    write_facade->add_statement("5", StatementType::Assign);

    write_facade->add_variable("a");
    write_facade->add_variable("b");
    write_facade->add_statement_modify_var("1", "a");
    write_facade->add_statement_modify_var("2", "b");
    write_facade->add_statement_modify_var("3", "a");
    write_facade->add_statement_modify_var("4", "b");

    write_facade->finalise_pkb();

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Modifies(s1, v)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ModifiesS{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4"});
    }

    SECTION("Evaluate - Select a such that Modifies(a, v)") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ModifiesS{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select r such that Modifies(r, v)") {
        const auto query = Query{
            std::make_shared<ReadSynonym>("r"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ModifiesS{std::make_shared<ReadSynonym>("r"), std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2"});
    }

    SECTION("Evaluate - Select v such that Modifies(if1, v)") {
        const auto query = Query{
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ModifiesS{std::make_shared<IfSynonym>("if1"), std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"a"});
    }

    SECTION("Evaluate - Select v such that Modifies(w, v)") {
        const auto query = Query{
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ModifiesS{std::make_shared<WhileSynonym>("w"), std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"b"});
    }

    SECTION("Evaluate - Select s1 such that Modifies(s1, \"a\")") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesS{std::make_shared<AnyStmtSynonym>("s1"), QuotedIdent{"a"}},
                                                 false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "3"});
    }

    SECTION("Evaluate - Select s1 such that Modifies(s1, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesS{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4"});
    }

    SECTION("Evaluate - Select v such that Modifies(3, v)") {
        const auto query = Query{
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesS{Integer{"3"}, std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"a"});
    }

    SECTION("Evaluate - Select s1 such that Modifies(3, \"a\")") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesS{Integer{"3"}, QuotedIdent{"a"}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }
    SECTION("Evaluate - Select s1 such that Modifies(3, \"b\")") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesS{Integer{"3"}, QuotedIdent{"b"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Modifies(3, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesS{Integer{"3"}, WildCard{}}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Modifies(5, _)") {
        const auto query = Query{
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesS{Integer{"5"}, WildCard{}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }
}