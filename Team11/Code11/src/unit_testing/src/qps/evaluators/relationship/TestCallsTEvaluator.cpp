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

TEST_CASE("Test Evaluator Calls*") {
    const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

    // Populate PkbManager
    write_facade->add_procedure("proc1");
    write_facade->add_procedure("proc2");
    write_facade->add_procedure("proc3");

    write_facade->add_calls("proc1", "proc2");
    write_facade->add_calls("proc2", "proc3");

    write_facade->finalise_pkb({"proc3", "proc2", "proc1"});

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select p such that Calls*(p, q)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
                std::make_shared<ProcSynonym>("q"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    CallsT{std::make_shared<ProcSynonym>("p"), std::make_shared<ProcSynonym>("q")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2"});
    }

    SECTION("Evaluate - Select q such that Calls*(p, q)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
                std::make_shared<ProcSynonym>("q"),
            },
            std::make_shared<ProcSynonym>("q"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    CallsT{std::make_shared<ProcSynonym>("p"), std::make_shared<ProcSynonym>("q")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc3", "proc2"});
    }

    SECTION("Evaluate - Select p such that Calls*(p, \"q\") - Should be transitive") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{std::make_shared<ProcSynonym>("p"), QuotedIdent("proc3")},
                                                 false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2"});
    }

    SECTION("Evaluate - Select p such that Calls*(p, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{std::make_shared<ProcSynonym>("p"), WildCard()}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2"});
    }

    SECTION("Evaluate - Select q such that Calls*(\"p\", q) - Should be transitive") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("q"),
            },
            std::make_shared<ProcSynonym>("q"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{QuotedIdent("proc1"), std::make_shared<ProcSynonym>("q")},
                                                 false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc2", "proc3"});
    }

    SECTION("Evaluate - Select p such that Calls*(\"p\", \"q\") returns all procedures if true") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{QuotedIdent("proc1"), QuotedIdent("proc3")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2", "proc3"});
    }

    SECTION("Evaluate - Select p such that Calls*(\"p\", \"q\") returns no procedures if false") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{QuotedIdent("proc3"), QuotedIdent("proc1")}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select p such that Calls*(\"p\", _) returns all procedures if true") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{QuotedIdent("proc1"), WildCard()}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2", "proc3"});
    }

    SECTION("Evaluate - Select p such that Calls*(\"p\", _) returns no procedures if false") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{QuotedIdent("proc3"), WildCard()}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select q such that Calls*(_, q)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("q"),
            },
            std::make_shared<ProcSynonym>("q"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{WildCard(), std::make_shared<ProcSynonym>("q")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc3", "proc2"});
    }

    SECTION("Evaluate - Select p such that Calls*(_, \"q\") returns all procedures if true") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{WildCard(), QuotedIdent("proc2")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2", "proc3"});
    }

    SECTION("Evaluate - Select p such that Calls*(_, \"q\") returns no procedures if false") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{WildCard(), QuotedIdent("proc1")}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select p such that Calls*(_, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(CallsT{WildCard(), WildCard()}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2", "proc3"});
    }
}