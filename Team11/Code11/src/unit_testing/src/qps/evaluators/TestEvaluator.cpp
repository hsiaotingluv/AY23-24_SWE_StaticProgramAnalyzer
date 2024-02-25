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

TEST_CASE("Test Evaluator Parent") {
    const auto &[read_facade, write_facade] = PKB::create_facades();

    // Populate PKB
    const auto if_strs = std::vector<std::string>{"1"};
    const auto while_strs = std::vector<std::string>{"3"};
    const auto assign_strs = std::vector<std::string>{"2", "4", "5"};

    for (const auto &x: if_strs) {
        write_facade->add_statement(x, StatementType::If);
    }
    for (const auto &x: while_strs) {
        write_facade->add_statement(x, StatementType::While);
    }
    for (const auto &x: assign_strs) {
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
                                Parent{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
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
                        std::make_shared<SuchThatClause>(
                                Parent{std::make_shared<AnyStmtSynonym>("s1"), Integer{4}}),
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
                        std::make_shared<SuchThatClause>(
                                Parent{std::make_shared<AnyStmtSynonym>("s1"), Integer{5}}),
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
                        std::make_shared<SuchThatClause>(
                                Parent{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
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
                        std::make_shared<SuchThatClause>(
                                Parent{Integer{3}, std::make_shared<AnyStmtSynonym>("s2")}),
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
                        std::make_shared<SuchThatClause>(
                                Parent{Integer{3}, Integer{4}}),
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
                    std::make_shared<SuchThatClause>(
                    Parent{Integer{3}, Integer{5}}),
                },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(3, _)") {
        const auto query = Query{
                Synonyms{
                    std::make_shared<AnyStmtSynonym>("s1")
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                Parent{Integer{3}, WildCard{}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select s2 such that Parent(_, s2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s2")
                },
                std::make_shared<AnyStmtSynonym>("s2"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                Parent{WildCard{}, std::make_shared<AnyStmtSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "4"});
    }

    SECTION("Evaluate - Select s1 such that Parent(_, 2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1")
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                Parent{WildCard{}, Integer{2}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Parent(_, 3)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1")
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                Parent{WildCard{}, Integer{3}}),
                },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(_, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1")
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                Parent{WildCard{}, WildCard{}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5"});
    }
}

TEST_CASE("Test Evaluator Parent*") {
    const auto &[read_facade, write_facade] = PKB::create_facades();

    // Populate PKB
    const auto if_strs = std::vector<std::string>{"1", "2"};
    const auto while_strs = std::vector<std::string>{"4", "5"};
    const auto assign_strs = std::vector<std::string>{"3", "6", "7"};

    for (const auto &x: if_strs) {
        write_facade->add_statement(x, StatementType::If);
    }
    for (const auto &x: while_strs) {
        write_facade->add_statement(x, StatementType::While);
    }
    for (const auto &x: assign_strs) {
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
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                        std::make_shared<AnyStmtSynonym>("s2"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(if1, s2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<IfSynonym>("s1"),
                        std::make_shared<AnyStmtSynonym>("s2"),
                },
                std::make_shared<IfSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, if2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                        std::make_shared<IfSynonym>("s2"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<IfSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(while1, s2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<WhileSynonym>("s1"),
                        std::make_shared<AnyStmtSynonym>("s2"),
                },
                std::make_shared<WhileSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{std::make_shared<WhileSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"4", "5"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, while2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                        std::make_shared<WhileSynonym>("s2"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<WhileSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"4"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, 3)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{std::make_shared<AnyStmtSynonym>("s1"), Integer{3}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, 7)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{std::make_shared<AnyStmtSynonym>("s1"), Integer{7}}),
                },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent*(s1, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "4", "5"});
    }

    SECTION("Evaluate - Select s2 such that Parent*(1, s2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s2"),
                },
                std::make_shared<AnyStmtSynonym>("s2"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{Integer{1}, std::make_shared<AnyStmtSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(1, 3)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{Integer{1}, Integer{3}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(1, 7)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{Integer{1}, Integer{7}}),
                },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent*(1, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1")
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{Integer{1}, WildCard{}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
    }

    SECTION("Evaluate - Select s2 such that Parent*(_, s2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s2")
                },
                std::make_shared<AnyStmtSynonym>("s2"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{WildCard{}, std::make_shared<AnyStmtSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3", "5", "6"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(_, 2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1")
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{WildCard{}, Integer{2}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
    }

    SECTION("Evaluate - Select s1 such that Parent*(_, 7)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1")
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{WildCard{}, Integer{7}}),
                },
        };

        const auto output = evaluator.evaluate(query);
        REQUIRE(output.empty());
    }

    SECTION("Evaluate - Select s1 such that Parent(_, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1")
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                ParentT{WildCard{}, WildCard{}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2", "3", "4", "5", "6", "7"});
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
                std::make_shared<SuchThatClause>(Follows{Integer{"1"}, std::make_shared<AnyStmtSynonym>("s2")}),
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
                std::make_shared<SuchThatClause>(Follows{std::make_shared<AnyStmtSynonym>("s2"), Integer{"2"}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1"});
    }

    SECTION("Evaluate - Select s2 such that Follows (s2, 10)") {
        const auto query = Query{
            Synonyms{
                std::make_shared<AnyStmtSynonym>("s2"),
            },
            std::make_shared<AnyStmtSynonym>("s2"),
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<AnyStmtSynonym>("s2"), Integer{"10"}}),
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
                std::make_shared<SuchThatClause>(Follows{Integer{"1"}, Integer{"2"}}),
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
                std::make_shared<SuchThatClause>(Follows{Integer{"1"}, WildCard{}}),
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
                std::make_shared<SuchThatClause>(Follows{WildCard{}, Integer{"3"}}),
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
                std::make_shared<SuchThatClause>(Follows{Integer{"1"}, Integer{"3"}}),
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
                std::make_shared<SuchThatClause>(Follows{Integer{"2"}, Integer{"10"}}),
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
                std::make_shared<SuchThatClause>(Follows{Integer{"2"}, Integer{"2"}}),
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

TEST_CASE("Test Evaluator Follows*"){
    const auto& [read_facade, write_facade] = PKB::create_facades();

    // Populate PKB
    const auto assign_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
    for (const auto& x : assign_strs) {
        write_facade->add_statement(x, StatementType::Assign);
    }
    write_facade->add_follows("1", "2");
    write_facade->add_follows("2", "3");
    write_facade->finalise_pkb();

    auto evaluator = QueryEvaluator{read_facade};

    SECTION("Evaluate - Select s1 such that Follows* (s1, s2)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                        std::make_shared<AnyStmtSynonym>("s2"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (s1, 3)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{std::make_shared<AnyStmtSynonym>("s1"), Integer{3}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (s1, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"1", "2"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (1, s1)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{Integer{1}, std::make_shared<AnyStmtSynonym>("s1")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (1, 3)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{Integer{1}, Integer{3}}),
                },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Follows* (1, 5)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{Integer{1}, Integer{5}}),
                },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Follows* (1, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{Integer{1}, WildCard{}}),
                },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Follows* (5, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{Integer{5}, WildCard{}}),
                },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Follows* (10, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{Integer{10}, WildCard{}}),
                },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Follows* (_, s1)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{ WildCard{}, std::make_shared<AnyStmtSynonym>("s1")}),
                },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{"2", "3"});
    }

    SECTION("Evaluate - Select s1 such that Follows* (_, 3)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{ WildCard{}, Integer{3}}),
                },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - Select s1 such that Follows* (_, 1)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{ WildCard{}, Integer{1}}),
                },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

    SECTION("Evaluate - Select s1 such that Follows* (_, _)") {
        const auto query = Query{
                Synonyms{
                        std::make_shared<AnyStmtSynonym>("s1"),
                },
                std::make_shared<AnyStmtSynonym>("s1"),
                std::vector<std::shared_ptr<Clause>>{
                        std::make_shared<SuchThatClause>(
                                FollowsT{ WildCard{}, WildCard{}}),
                },
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

}

TEST_CASE("Test Evaluator UsesS"){
    const auto& [read_facade, write_facade] = PKB::create_facades();

    // Populate PKB
    write_facade->add_statement("1", StatementType::Assign);
    write_facade->add_statement("2", StatementType::Print);
    write_facade->add_statement("3", StatementType::If);
    write_facade->add_statement("4", StatementType::While);
    write_facade->add_statement("5", StatementType::Assign);

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
                        std::make_shared<SuchThatClause>(
                                UsesS{std::make_shared<AnyStmtSynonym>("s1"), QuotedIdent{"a"}}),
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
                        std::make_shared<SuchThatClause>(
                                UsesS{std::make_shared<AnyStmtSynonym>("s1"), WildCard{}}),
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
                        std::make_shared<SuchThatClause>(
                                UsesS{Integer{3}, std::make_shared<VarSynonym>("v")}),
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
                        std::make_shared<SuchThatClause>(
                                UsesS{Integer{3}, QuotedIdent{"a"}}),
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
                        std::make_shared<SuchThatClause>(
                                UsesS{Integer{3}, QuotedIdent{"b"}}),
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
                        std::make_shared<SuchThatClause>(
                                UsesS{Integer{3}, WildCard{}}),
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
                        std::make_shared<SuchThatClause>(
                                UsesS{Integer{5}, WildCard{}}),
                },
        };

        REQUIRE(evaluator.evaluate(query).empty());
    }

}