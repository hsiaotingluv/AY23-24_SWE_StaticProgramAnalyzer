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

TEST_CASE("Test Evaluator ModifiesP") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    write_facade->add_procedure("proc1");
    write_facade->add_procedure("proc2");
    write_facade->add_procedure("proc3");
    write_facade->add_variable("a");
    write_facade->add_variable("b");
    write_facade->add_variable("c");

    write_facade->add_procedure_modify_var("proc1", "a");
    write_facade->add_procedure_modify_var("proc2", "b");

    write_facade->finalise_pkb();

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select p such that Modifies(p, v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ModifiesP{std::make_shared<ProcSynonym>("p"), std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2"});
    }

    SECTION("Evaluate - Select v such that Modifies(p, v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ModifiesP{std::make_shared<ProcSynonym>("p"), std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"a", "b"});
    }

    SECTION("Evaluate - Select p such that Modifies(p, \"a\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesP{std::make_shared<ProcSynonym>("p"), qps::QuotedIdent("a")},
                                                 false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1"});
    }

    SECTION("Evaluate - Select p such that Modifies(p, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    ModifiesP{std::make_shared<ProcSynonym>("p"), std::make_shared<VarSynonym>("v")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2"});
    }

    SECTION("Evaluate - Select v such that Modifies(\"p\", v)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesP{QuotedIdent("proc1"), std::make_shared<VarSynonym>("v")},
                                                 false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"a"});
    }

    SECTION("Evaluate - Select p such that Modifies(\"p\", \"v\") returns all procedures if true") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesP{QuotedIdent("proc1"), QuotedIdent("a")}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2", "proc3"});
    }

    SECTION("Evaluate - Select p such that Modifies(\"p\", \"v\") returns no procedures if false") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesP{QuotedIdent("proc3"), QuotedIdent("b")}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select p such that Modifies(\"p\", _) returns all procedures if true") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesP{QuotedIdent("proc1"), WildCard()}, false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"proc1", "proc2", "proc3"});
    }

    SECTION("Evaluate - Select p such that Modifies(\"p\", _) returns no procedures if false") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesP{QuotedIdent("proc3"), WildCard()}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }
}