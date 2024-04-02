#include "test_evaluator.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb_manager.h"

#include "qps/evaluators/query_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"
#include "qps/parser/expression_parser.hpp"

#include <memory>
#include <vector>

using namespace qps;

using namespace pkb;

TEST_CASE("Test Evaluator Pattern") {
    const auto& [read_facade, write_facade] = PkbManager::create_facades();

    // Populate PkbManager
    const auto assign_strs = std::vector<std::string>{"1", "2"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }

    write_facade->add_variable("x");
    write_facade->add_variable("y");
    write_facade->add_variable("z");

    write_facade->add_constant("2");

    write_facade->add_assignment("1", "x", "y z 2 * +");
    write_facade->add_statement_modify_var("1", "x");

    write_facade->add_assignment("2", "z", "1");
    write_facade->add_statement_modify_var("2", "z");
    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select a such that Pattern a (\"x\", _)") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, WildCard{}}, false),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate - Select a such that Pattern a (\"y\", _)") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"y"}, WildCard{}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (\"x\", \"y + z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, ExactMatch{"y z 2 * +"}},
                    false),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (\"x\", \"z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, ExactMatch{"z 2 *"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (_, \"y + z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, ExactMatch{"y z 2 * +"}},
                    false),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (_, \"z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, ExactMatch{"z 2 *"}}, false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (x, \"z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(PatternAssign{std::make_shared<AssignSynonym>("a"),
                                                              std::make_shared<VarSynonym>("x"), ExactMatch{"z 2 *"}},
                                                false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (\"x\", \"y + z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, PartialMatch{"y z 2 * +"}},
                    false),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (\"x\", \"z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, PartialMatch{"z 2 *"}},
                    false),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (\"x\", \"y + z\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, PartialMatch{"y z +"}},
                    false),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (\"x\", \"z * 2 + y\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(PatternAssign{std::make_shared<AssignSynonym>("a"),
                                                              std::make_shared<VarSynonym>("v"),
                                                              PartialMatch{"y z 2 * +"}},
                                                false),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select a such that Pattern a (v, _)") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v"), WildCard{}},
                    false),
            },
        };

        const auto expected = std::vector<std::string>{"1", "2"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate - Select a such that Pattern a (v, _)") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v"), WildCard{}},
                    false),
            },
        };

        const auto expected = std::vector<std::string>{"1", "2"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate - Select a such that Pattern a (_, _)") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), WildCard{}, WildCard{}}, false),
            },
        };

        const auto expected = std::vector<std::string>{"1", "2"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (_, \"y + z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), WildCard{}, ExactMatch{"y z 2 * +"}}, false),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (_, \"z * 2\")") {
        const auto query = Query{
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a"), WildCard{}, PartialMatch{"z 2 *"}}, false),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }
}
