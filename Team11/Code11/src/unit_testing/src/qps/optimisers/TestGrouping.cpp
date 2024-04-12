#include "catch.hpp"

#include "qps/optimisers/grouping.hpp"
#include "qps/optimisers/optimiser.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/attribute.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"
#include "qps/parser/expression_parser.hpp"
#include "utils.hpp"
#include <memory>
#include <variant>
#include <vector>

using namespace qps;

TEST_CASE("Test GroupingOptimiser - Group connected clauses") {
    const std::shared_ptr<Optimiser> optimiser = std::make_shared<GroupingOptimiser>();

    SECTION("Grouping - No connected clause") {
        const auto query = Query{
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
                std::make_shared<SuchThatClause>(UsesS{Integer{"1"}, std::make_shared<VarSynonym>("v")}, false),

            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 2); // Split into 2 queries, since the clauses are not related
    }

    SECTION("Grouping - 1 connected clause") {
        const auto query = Query{
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
                std::make_shared<SuchThatClause>(Next{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
                std::make_shared<SuchThatClause>(UsesS{Integer{"1"}, std::make_shared<VarSynonym>("v")}, false),

            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 2); // Grouped into 2 queries;
    }
}

TEST_CASE("Grouping - Complex Queries") {
    const std::shared_ptr<Optimiser> optimiser = std::make_shared<GroupingOptimiser>();

    SECTION("Grouping - OPT 2") {
        // Taken from the OPT2 query in the worksheet
        // assign a, a1, a2; stmt s1, s2, s3; variable v1, v2;
        // Select <s1, a, a1, v2>
        // such that Uses (5, "y") and Uses (s1, v1) and
        // Affects (a1, a2) with a1.stmt = 20
        // such that Parent (s1, s2)
        // pattern a2 ("a", _) such that Next (s2, s3)
        // and Modifies (s1, "x")
        // and Modifies (a, v2) pattern a (v1, "x")

        const auto query = Query{
            std::vector<Elem>{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AssignSynonym>("a"),
                              std::make_shared<AssignSynonym>("a1"), std::make_shared<VarSynonym>("v2")},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{Integer{"5"}, QuotedIdent{"y"}}, false),
                std::make_shared<SuchThatClause>(
                    UsesS{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<VarSynonym>("v1")}, false),
                std::make_shared<SuchThatClause>(
                    Affects{std::make_shared<AssignSynonym>("a1"), std::make_shared<AssignSynonym>("a2")}, false),
                std::make_shared<WithClause>(
                    TypedRef{AttrRef(std::make_shared<AssignSynonym>("a1"), StmtNum{}, AttrRef::Type::Integer)},
                    TypedRef{Integer{"20"}}, false),
                std::make_shared<SuchThatClause>(
                    Parent{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a2"), QuotedIdent{"a"}, WildCard{}}, false),
                std::make_shared<SuchThatClause>(
                    Next{std::make_shared<AnyStmtSynonym>("s2"), std::make_shared<AnyStmtSynonym>("s3")}, false),
                std::make_shared<SuchThatClause>(ModifiesS{std::make_shared<AnyStmtSynonym>("s1"), QuotedIdent{"x"}},
                                                 false),
                std::make_shared<SuchThatClause>(
                    ModifiesS{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v2")}, false),
                std::make_shared<PatternClause>(PatternAssign{std::make_shared<AssignSynonym>("a"),
                                                              std::make_shared<VarSynonym>("v1"), ExactMatch{"x"}},
                                                false),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 3); // Grouped into 3 queries;

        const auto query1 = results[0];
        require_boolean(query1.reference);
        REQUIRE(query1.clauses.size() == 1);

        const auto query2 = results[1];
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query2.reference));
        const auto reference2 = std::get<std::vector<Elem>>(query2.reference);
        REQUIRE(reference2.size() == 1);
        require_value<AssignSynonym>(reference2, "a1");
        REQUIRE(query2.clauses.size() == 3);

        const auto query3 = results[2];
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query3.reference));
        const auto reference3 = std::get<std::vector<Elem>>(query3.reference);
        REQUIRE(reference3.size() == 3);
        require_value<AssignSynonym>(reference3, "a");
        require_value<VarSynonym>(reference3, "v2");
        require_value<AnyStmtSynonym>(reference3, "s1");
        REQUIRE(query3.clauses.size() == 6);
    }

    SECTION("Grouping - OPT 2 Modified") {
        // Modified from the OPT2 query in the worksheet
        // assign a, a1, a2; stmt s1, s2, s3; variable v1, v2;
        // Select <s1, a, a1, v2>
        // such that Uses (5, "y") and
        // Affects (a1, a2) with a1.stmt = 20
        // such that Parent (s1, s2)
        // pattern a2 ("a", _) such that Next (s2, s3)
        // and Modifies (s1, "x")
        // and Modifies (a, v2) pattern a (v1, "x")
        // and Affects (s1, a)

        const auto query = Query{
            std::vector<Elem>{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AssignSynonym>("a"),
                              std::make_shared<AssignSynonym>("a1"), std::make_shared<VarSynonym>("v2")},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(UsesS{Integer{"5"}, QuotedIdent{"y"}}, false),
                std::make_shared<SuchThatClause>(
                    Affects{std::make_shared<AssignSynonym>("a1"), std::make_shared<AssignSynonym>("a2")}, false),
                std::make_shared<WithClause>(
                    TypedRef{AttrRef(std::make_shared<AssignSynonym>("a1"), StmtNum{}, AttrRef::Type::Integer)},
                    TypedRef{Integer{"20"}}, false),
                std::make_shared<SuchThatClause>(
                    Parent{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AnyStmtSynonym>("s2")}, false),
                std::make_shared<PatternClause>(
                    PatternAssign{std::make_shared<AssignSynonym>("a2"), QuotedIdent{"a"}, WildCard{}}, false),
                std::make_shared<SuchThatClause>(
                    Next{std::make_shared<AnyStmtSynonym>("s2"), std::make_shared<AnyStmtSynonym>("s3")}, false),
                std::make_shared<SuchThatClause>(ModifiesS{std::make_shared<AnyStmtSynonym>("s1"), QuotedIdent{"x"}},
                                                 false),
                std::make_shared<SuchThatClause>(
                    ModifiesS{std::make_shared<AssignSynonym>("a"), std::make_shared<VarSynonym>("v2")}, false),
                std::make_shared<PatternClause>(PatternAssign{std::make_shared<AssignSynonym>("a"),
                                                              std::make_shared<VarSynonym>("v1"), ExactMatch{"x"}},
                                                false),
                std::make_shared<SuchThatClause>(
                    Affects{std::make_shared<AnyStmtSynonym>("s1"), std::make_shared<AssignSynonym>("a")}, false),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 3); // Grouped into 3 queries;

        const auto query1 = results[0];
        require_boolean(query1.reference);
        REQUIRE(query1.clauses.size() == 1);

        const auto query2 = results[1];
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query2.reference));
        const auto reference2 = std::get<std::vector<Elem>>(query2.reference);
        REQUIRE(reference2.size() == 1);
        require_value<AssignSynonym>(reference2, "a1");
        REQUIRE(query2.clauses.size() == 3);

        const auto query3 = results[2];
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query3.reference));
        const auto reference3 = std::get<std::vector<Elem>>(query3.reference);
        REQUIRE(reference3.size() == 3);
        require_value<AssignSynonym>(query3.reference, "a");
        require_value<VarSynonym>(query3.reference, "v2");
        require_value<AnyStmtSynonym>(query3.reference, "s1");
        REQUIRE(query3.clauses.size() == 6);
    }
}