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

TEST_CASE("Test Evaluator UsesS") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    write_facade->add_statement("1", StatementType::Assign);
    write_facade->add_statement("2", StatementType::Print);
    write_facade->add_statement("3", StatementType::If);
    write_facade->add_statement("4", StatementType::While);
    write_facade->add_statement("5", StatementType::Assign);

    write_facade->add_variable("a");
    write_facade->add_variable("b");
    write_facade->add_statement_uses_var("1", "a");
    write_facade->add_statement_uses_var("2", "b");
    write_facade->add_statement_uses_var("3", "a");
    write_facade->add_statement_uses_var("4", "b");

    write_facade->finalise_pkb();

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Uses(s1, v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    UsesS{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<VarSynonym>("v")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4"});
    }

    SECTION("Evaluate - Select a such that Uses(a, v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    UsesS{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select pn such that Uses(pn, v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<PrintSynonym>("pn"),
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<PrintSynonym>("pn"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    UsesS{std::make_shared<PrintSynonym>("pn"), std::make_shared<VarSynonym>("v")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2"});
    }

    SECTION("Evaluate - Select v such that Uses(if1, v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<IfSynonym>("if1"),
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    UsesS{std::make_shared<IfSynonym>("if1"), std::make_shared<VarSynonym>("v")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"a"});
    }

    SECTION("Evaluate - Select v such that Uses(w, v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<WhileSynonym>("w"),
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    UsesS{std::make_shared<WhileSynonym>("w"), std::make_shared<VarSynonym>("v")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"b"});
    }

    SECTION("Evaluate - Select s1 such that Uses(s1, \"a\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{std::make_shared<AnyStmtSynonym>("s1"), QuotedIdent{"a"}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "3"});
    }

    SECTION("Evaluate - Select s1 such that Uses(s1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4"});
    }

    SECTION("Evaluate - Select v such that Uses(3, v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{Integer{"3"}, std::make_shared<VarSynonym>("v")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"a"});
    }

    SECTION("Evaluate - Select s1 such that Uses(3, \"a\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{Integer{"3"}, QuotedIdent{"a"}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }
    SECTION("Evaluate - Select s1 such that Uses(3, \"b\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{Integer{"3"}, QuotedIdent{"b"}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Uses(3, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{Integer{"3"}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Uses(5, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{Integer{"5"}, WildCard{}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }
}
