#include "catch.hpp"

#include "utils.hpp"

#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"

#include "qps/parser/untyped/untyped_parser.hpp"

#include <variant>

using namespace qps;

TEST_CASE("Test QPSParser") {
    const auto parser = untyped::DefaultUntypedParser{};

    SECTION("Query with stmt-stmt relationship") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(13, s)";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::UntypedQuery>>(output));
        const auto& [declarations, untyped] = std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(output);

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

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::UntypedQuery>>(output));
        const auto& [declarations, untyped] = std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(output);

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

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::UntypedQuery>>(output));
        const auto& [declarations, untyped] = std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(output);

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

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::UntypedQuery>>(output));
        const auto& [declarations, untyped] = std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(output);

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

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::UntypedQuery>>(output));
        const auto& [declarations, untyped] = std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(output);

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

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::UntypedQuery>>(output));
        const auto& [declarations, untyped] = std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<AssignSynonym>(declarations[0], "a");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"a"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[0]));
        const auto pattern_clause = std::get<untyped::UntypedPatternClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedPatternClause{untyped::UntypedSynonym{IDENT{"a"}}, WildCard{},
                                                                    ExpressionSpec{PartialMatch{"count 1 + "}}};
        REQUIRE(pattern_clause == reference_clause);
    }

    SECTION("Query with pattern clause 2") {
        const auto query = R"(assign newa;Select newa pattern newa ( "normSq" , _"cenX * cenX"_))";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::UntypedQuery>>(output));
        const auto& [declarations, untyped] = std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<AssignSynonym>(declarations[0], "newa");

        const auto& [reference, clauses] = untyped;

        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"newa"}});

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[0]));
        const auto pattern_clause = std::get<untyped::UntypedPatternClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedPatternClause{
            untyped::UntypedSynonym{IDENT{"newa"}}, QuotedIdent{"normSq"}, ExpressionSpec{PartialMatch{"cenX cenX * "}}};
        REQUIRE(pattern_clause == reference_clause);
    }

    SECTION("Query with undefined synonym") {
        const auto query = "procedure p; Select v";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::UntypedQuery>>(output));
        const auto& [declarations, untyped] = std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<ProcSynonym>(declarations[0], "p");

        const auto& [reference, clauses] = untyped;
        REQUIRE(reference == untyped::UntypedSynonym{IDENT{"v"}});
        REQUIRE(clauses.empty());
    }

    SECTION("Modifies cannot start with wildcard") {
        const auto query = R"(procedure p; variable v; Select p such that Modifies("x", v))";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<SyntaxError>(output));
    }

    SECTION("Uses cannot start with quoted ident") {
        const auto query = R"(procedure p; variable v; Select p such that Uses("s", p))";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));
    }

    SECTION("Uses cannot start with integer") {
        const auto query = "variable v; Select v such that Uses(v, 1)";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));
    }
}

TEST_CASE("Test Parser - Basic Syntax Issues") {
    const auto parser = untyped::DefaultUntypedParser{};
    SECTION("Missing synonym") {
        const auto query = "variable v,";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));

        const auto query2 = "variable v,+";
        const auto output2 = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));
    }

    SECTION("Wrong Keyword") {
        const auto query = "variable v; select v such that Uses(v, 1)";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output)); // Select should be in uppercase

        const auto query2 = "variable v; Select + such that Uses(v, 1) such that Uses(v, 1)";
        const auto output2 = parser.parse(query2);
        REQUIRE(std::holds_alternative<SyntaxError>(output)); // + is not a valid keyword

        const auto query3 = "variable v; Select s such that Uses(v, 1";
        const auto output3 = parser.parse(query3);
        REQUIRE(std::holds_alternative<SyntaxError>(output)); // Missing closing bracket

        const auto query4 = "variable v; Select s such that Uses(v, 1(";
        const auto output4 = parser.parse(query4);
        REQUIRE(std::holds_alternative<SyntaxError>(output)); // Wrong closing bracket

        const auto query5 = R"(variable v; Select s such that Uses("v", 1))";
        const auto output5 = parser.parse(query5);
        REQUIRE(std::holds_alternative<SyntaxError>(output)); // "v" is not a valid synonym
    }
}