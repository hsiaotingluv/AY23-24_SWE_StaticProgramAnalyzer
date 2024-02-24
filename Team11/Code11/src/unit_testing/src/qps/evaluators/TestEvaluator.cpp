#include "catch.hpp"

#include "common/statement_type.hpp"
#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb.h"

#include "qps/evaluators/query_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/expression_parser.hpp"
#include <algorithm>
#include <memory>
#include <vector>

using namespace qps;

auto require_equal(const std::vector<std::string>& actual, const std::vector<std::string>& expected) -> void {
    REQUIRE(actual.size() == expected.size());
    for (const auto& value : actual) {
        auto it = std::find(expected.begin(), expected.end(), value);
        REQUIRE(it != expected.end());
    }
}

TEST_CASE("Test Evaluator Entities") {
    SECTION("Evaluate - All Variables") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto variable_strs = std::vector<std::string>{"v", "w", "x", "y", "z"};
        for (const auto& x : variable_strs) {
            write_facade->add_variable(x);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), variable_strs);
    }

    SECTION("Evaluate - All Procedures") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto procedure_strs = std::vector<std::string>{"p", "q", "r", "s", "t"};
        for (const auto& x : procedure_strs) {
            write_facade->add_procedure(x);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), procedure_strs);
    }

    SECTION("Evaluate - All Constants") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto constant_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : constant_strs) {
            write_facade->add_constant(x);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ConstSynonym>("c"),
            },
            std::make_shared<ConstSynonym>("c"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), constant_strs);
    }

    SECTION("Evaluate - All Assigns") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto assign_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : assign_strs) {
            write_facade->add_statement(x, StatementType::Assign);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - All Ifs") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto if_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : if_strs) {
            write_facade->add_statement(x, StatementType::If);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<IfSynonym>("i"),
            },
            std::make_shared<IfSynonym>("i"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), if_strs);
    }

    SECTION("Evaluate - All Whiles") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto while_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : while_strs) {
            write_facade->add_statement(x, StatementType::While);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<WhileSynonym>("w"),
            },
            std::make_shared<WhileSynonym>("w"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), while_strs);
    }

    SECTION("Evaluate - All Calls") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto call_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : call_strs) {
            write_facade->add_statement(x, StatementType::Call);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<CallSynonym>("c"),
            },
            std::make_shared<CallSynonym>("c"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), call_strs);
    }

    SECTION("Evaluate - All Prints") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto print_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : print_strs) {
            write_facade->add_statement(x, StatementType::Print);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<PrintSynonym>("p"),
            },
            std::make_shared<PrintSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), print_strs);
    }

    SECTION("Evaluate - All Reads") {
        const auto& [read_facade, write_facade] = PKB::create_facades();

        // Populate PKB
        const auto read_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : read_strs) {
            write_facade->add_statement(x, StatementType::Read);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ReadSynonym>("r"),
            },
            std::make_shared<ReadSynonym>("r"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), read_strs);
    }
}

TEST_CASE("Test Evaluator Follows") {
    const auto& [read_facade, write_facade] = PKB::create_facades();

    // Populate PKB
    const auto assign_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }
    write_facade->add_follows("1", "2");
    write_facade->add_follows("2", "3");

    auto evaluator = QueryEvaluator{read_facade};
    SECTION("Evaluate - Select s1 such that Follows (s1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Follows{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s2 such that Follows (1, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{1}, std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2"});
    }

    SECTION("Evaluate - Select s2 such that Follows (s2, 2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<AnyStmtSynonym>("s2"), Integer{2}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select s2 such that Follows (s2, 2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<AnyStmtSynonym>("s2"), Integer{10}}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s2 such that Follows (1, 2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{1}, Integer{2}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s2 such that Follows (1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{1}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s2 such that Follows (_, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{WildCard{}, Integer{3}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s2 such that Follows (_, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{WildCard{}, std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s2 such that Follows (_, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{WildCard{}, WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Follows (s1, _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s1"),
            },
            std::make_shared<AnyStmtSynonym>("s1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s2 such that Follows (1, 3)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{1}, Integer{3}}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s2 such that Follows (2, 10)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{2}, Integer{10}}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s2 such that Follows (2, 2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{Integer{2}, Integer{2}}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s2 such that Follows (s2, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Follows{std::make_shared<AnyStmtSynonym>("s2"), std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select w such that Follows (w, s2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<WhileSynonym>("w"),
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<WhileSynonym>("w"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Follows{std::make_shared<WhileSynonym>("w"), std::make_shared<AnyStmtSynonym>("s2")}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select w such that Follows (w1, w2)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<WhileSynonym>("w1"),
                std::make_shared<AnyStmtSynonym>("w2"),
            },
            std::make_shared<WhileSynonym>("w1"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    Follows{std::make_shared<WhileSynonym>("w1"), std::make_shared<WhileSynonym>("w2")}),
            },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }
}

TEST_CASE("Test Evaluator Pattern") {
    const auto& [read_facade, write_facade] = PKB::create_facades();

    // Populate PKB
    const auto assign_strs = std::vector<std::string>{"1", "2"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }

    write_facade->add_variable("x");
    write_facade->add_variable("y");
    write_facade->add_variable("z");

    write_facade->add_constant("2");

    write_facade->add_assignment("1", "x", "y z 2 * +");
    write_facade->add_statement_modifies_var("1", "x");

    write_facade->add_assignment("2", "z", "1");
    write_facade->add_statement_modifies_var("2", "z");
    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select a such that Pattern a (\"x\", _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"}, WildCard{}),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate - Select a such that Pattern a (\"y\", _)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"y"}, WildCard{}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (\"x\", \"y + z * 2\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"},
                                                ExactMatch{"y z 2 * +"}),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (\"x\", \"z * 2\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"},
                                                ExactMatch{"z 2 *"}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (_, \"y + z * 2\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"},
                                                ExactMatch{"y z 2 * +"}),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (_, \"z * 2\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"},
                                                ExactMatch{"z 2 *"}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (x, \"z * 2\")") {
        const auto query = Query{
            Synonyms{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("x")},
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("x"),
                                                ExactMatch{"z 2 *"}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (\"x\", \"y + z * 2\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"},
                                                PartialMatch{"y z 2 * +"}),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (\"x\", \"z * 2\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"},
                                                PartialMatch{"z 2 *"}),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (\"x\", \"y + z\")") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), QuotedIdent{"x"},
                                                PartialMatch{"y z +"}),
            },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (\"x\", \"z * 2 + y\")") {
        const auto query = Query{
            Synonyms{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v")},
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v"),
                                                PartialMatch{"y z 2 * +"}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select a such that Pattern a (v, _)") {
        const auto query = Query{
            Synonyms{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v")},
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v"),
                                                WildCard{}),
            },
        };

        const auto expected = std::vector<std::string>{"1", "2"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate - Select a such that Pattern a (v, _)") {
        const auto query = Query{
            Synonyms{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v")},
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v"),
                                                WildCard{}),
            },
        };

        const auto expected = std::vector<std::string>{"1", "2"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate - Select a such that Pattern a (_, _)") {
        const auto query = Query{
            Synonyms{std::make_shared<AssignSynonym>("a")},
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), WildCard{}, WildCard{}),
            },
        };

        const auto expected = std::vector<std::string>{"1", "2"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Exact Match - Select a such that Pattern a (_, \"y + z * 2\")") {
        const auto query = Query{
            Synonyms{std::make_shared<AssignSynonym>("a")},
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), WildCard{},
                                                ExactMatch{"y z 2 * +"}),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }

    SECTION("Evaluate Partial Match - Select a such that Pattern a (_, \"z * 2\")") {
        const auto query = Query{
            Synonyms{std::make_shared<AssignSynonym>("a")},
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<PatternClause>(std::make_shared<AssignSynonym>("a"), WildCard{},
                                                PartialMatch{"z 2 *"}),
            },
        };

        const auto expected = std::vector<std::string>{"1"};
        require_equal(evaluator.evaluate(query), expected);
    }
}
