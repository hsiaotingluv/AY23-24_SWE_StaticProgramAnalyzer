#include "catch.hpp"

#include "utils.hpp"

#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/entities/untyped/clause.hpp"
#include "qps/parser/entities/untyped/relationship.hpp"
#include "qps/parser/entities/untyped/synonym.hpp"

#include "qps/parser/parser.hpp"

#include <variant>

using namespace qps;

TEST_CASE("Test QPSParser") {
    const auto parser = QPSParser{};

    SECTION("Query with stmt-stmt relationship") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(13, s)";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto& [declarations, untyped] = output.value();

        REQUIRE(declarations.size() == 2);
        require_value<ProcSynonym>(declarations[0], "p");
        require_value<AnyStmtSynonym>(declarations[1], "s");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"s"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedStmtStmtRel{"Follows*", untyped::UntypedStmtRef{Integer{13}},
                                        untyped::UntypedStmtRef{untyped::UntypedSynonym{IDENT{"s"}}}}};
        REQUIRE(such_that_clause == reference_clause);
    }

    SECTION("Query with stmt-stmt relationship") {
        const auto query = " procedure p; stmt s; Select s such that Follows(s, 13)";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto& [declarations, untyped] = output.value();

        REQUIRE(declarations.size() == 2);

        require_value<ProcSynonym>(declarations[0], "p");
        require_value<AnyStmtSynonym>(declarations[1], "s");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"s"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedStmtStmtRel{"Follows", untyped::UntypedStmtRef{untyped::UntypedSynonym{IDENT{"s"}}},
                                        untyped::UntypedStmtRef{Integer{13}}}};
        REQUIRE(such_that_clause == reference_clause);
    };

    SECTION("Query with stmt-stmt relationship - indirect") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(s, 13)";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto& [declarations, untyped] = output.value();

        REQUIRE(declarations.size() == 2);

        require_value<ProcSynonym>(declarations[0], "p");
        require_value<AnyStmtSynonym>(declarations[1], "s");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"s"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedStmtStmtRel{"Follows*", untyped::UntypedStmtRef{untyped::UntypedSynonym{IDENT{"s"}}},
                                        untyped::UntypedStmtRef{Integer{13}}}};
        REQUIRE(such_that_clause == reference_clause);
    };

    SECTION("Query with stmt-ent relationship") {
        const auto query = R"(stmt s; Select s such that Uses(s, "v"))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto& [declarations, untyped] = output.value();

        REQUIRE(declarations.size() == 1);
        require_value<AnyStmtSynonym>(declarations[0], "s");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"s"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedRefEntRel{"Uses", untyped::UntypedRef{untyped::UntypedSynonym{IDENT{"s"}}},
                                      untyped::UntypedEntRef{QuotedIdent{"v"}}}};
        REQUIRE(such_that_clause == reference_clause);
    }

    SECTION("Query with ent-ent relationship for procedures") {
        const auto query = R"(procedure p; variable v; Select p such that Uses(p, "s"))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto& [declarations, untyped] = output.value();

        REQUIRE(declarations.size() == 2);
        require_value<ProcSynonym>(declarations[0], "p");
        require_value<VarSynonym>(declarations[1], "v");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"p"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedRefEntRel{"Uses", untyped::UntypedRef{untyped::UntypedSynonym{IDENT{"p"}}},
                                      untyped::UntypedEntRef{QuotedIdent{"s"}}}};
        REQUIRE(such_that_clause == reference_clause);
    }

    SECTION("Query with pattern clause") {
        const auto query = R"(        assign a;
Select a pattern a ( _ , _"count + 1"_))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();
        const auto& [declarations, untyped] = output.value();

        REQUIRE(declarations.size() == 1);
        require_value<AssignSynonym>(declarations[0], "a");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"a"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[0]));
        const auto pattern_clause = std::get<untyped::UntypedPatternClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedPatternClause{untyped::UntypedSynonym{IDENT{"a"}}, WildCard{},
                                                                    ExpressionSpec{PartialMatch{"count 1 +"}}};
        REQUIRE(pattern_clause == reference_clause);
    }

    SECTION("Query with pattern clause 2") {
        const auto query = R"(assign newa;Select newa pattern newa ( "normSq" , _"cenX * cenX"_))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        const auto& [declarations, untyped] = output.value();

        REQUIRE(declarations.size() == 1);
        require_value<AssignSynonym>(declarations[0], "newa");

        const auto& [reference, clauses] = untyped;

        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"newa"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[0]));
        const auto pattern_clause = std::get<untyped::UntypedPatternClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedPatternClause{
            untyped::UntypedSynonym{IDENT{"newa"}}, QuotedIdent{"normSq"}, ExpressionSpec{PartialMatch{"cenX cenX *"}}};
        REQUIRE(pattern_clause == reference_clause);
    }

    SECTION("Query with undefined synonym") {
        const auto query = "procedure p; Select v";
        const auto result = parser.parse(query);

        REQUIRE(result.has_value());
        const auto& [declarations, untyped] = result.value();

        REQUIRE(declarations.size() == 1);
        require_value<ProcSynonym>(declarations[0], "p");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"v"}});
        REQUIRE(clauses.empty());
    }

    SECTION("Modifies cannot start with wildcard") {
        const auto query = R"(procedure p; variable v; Select p such that Modifies("x", v))";
        const auto output = parser.parse(query);

        REQUIRE(!output.has_value());
    }

    SECTION("Uses cannot start with quoted ident") {
        const auto query = R"(procedure p; variable v; Select p such that Uses("s", p))";
        const auto output = parser.parse(query);

        REQUIRE(!output.has_value());
    }

    SECTION("Uses cannot start with integer") {
        const auto query3 = "variable v; Select v such that Uses(v, 1)";
        const auto output3 = parser.parse(query3);
        REQUIRE(!output3.has_value());
    }
}

TEST_CASE("Test Parser - Basic Syntax Issues") {
    const auto parser = QPSParser{};
    SECTION("Missing synonym") {
        const auto query = "variable v,";
        const auto output = parser.parse(query);
        REQUIRE(!output.has_value());

        const auto query2 = "variable v,+";
        const auto output2 = parser.parse(query);
        REQUIRE(!output2.has_value());
    }

    SECTION("Wrong Keyword") {
        const auto query = "variable v; select v such that Uses(v, 1)";
        const auto output = parser.parse(query);
        REQUIRE(!output.has_value()); // Select should be in uppercase

        const auto query2 = "variable v; Select + such that Uses(v, 1) such that Uses(v, 1)";
        const auto output2 = parser.parse(query2);
        REQUIRE(!output2.has_value()); // + is not a valid keyword

        const auto query3 = "variable v; Select s such that Uses(v, 1";
        const auto output3 = parser.parse(query3);
        REQUIRE(!output3.has_value()); // Missing closing bracket

        const auto query4 = "variable v; Select s such that Uses(v, 1(";
        const auto output4 = parser.parse(query4);
        REQUIRE(!output4.has_value()); // Wrong closing bracket

        const auto query5 = R"(variable v; Select s such that Uses("v", 1))";
        const auto output5 = parser.parse(query5);
        REQUIRE(!output5.has_value()); // "v" is not a valid synonym
    }
}