#include "catch.hpp"

#include "qps/parser/entities/attribute_name.hpp"
#include "utils.hpp"

#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"

#include "qps/parser/untyped/untyped_parser.hpp"

#include <variant>
#include <vector>

using namespace qps;

using UntypedVector = std::vector<std::variant<untyped::UntypedSynonym>>;

template <typename T>
auto require_value(const UntypedVector& vector, const std::string& str) -> void {
    const auto& value = T{IDENT{str}};
    for (const auto& elem : vector) {
        if (std::holds_alternative<T>(elem)) {
            REQUIRE(std::get<T>(elem) == value);
            return;
        }
    }
    REQUIRE(false);
}

TEST_CASE("Test QPSParser") {
    const auto parser = untyped::DefaultUntypedParser{};

    SECTION("Query with stmt-stmt relationship") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(13, s)";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 2);
        require_value<ProcSynonym>(declarations[0], "p");
        require_value<AnyStmtSynonym>(declarations[1], "s");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "s");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedStmtStmtRel{"Follows*", untyped::UntypedStmtRef{Integer{"13"}},
                                        untyped::UntypedStmtRef{untyped::UntypedSynonym{IDENT{"s"}}}}};
        REQUIRE(such_that_clause == reference_clause);
    }

    SECTION("Query with stmt-stmt relationship") {
        const auto query = " procedure p; stmt s; Select s such that Follows(s, 13)";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 2);

        require_value<ProcSynonym>(declarations[0], "p");
        require_value<AnyStmtSynonym>(declarations[1], "s");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "s");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedStmtStmtRel{"Follows", untyped::UntypedStmtRef{untyped::UntypedSynonym{IDENT{"s"}}},
                                        untyped::UntypedStmtRef{Integer{"13"}}}};
        REQUIRE(such_that_clause == reference_clause);
    };

    SECTION("Query with stmt-stmt relationship - indirect") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(s, 13)";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 2);

        require_value<ProcSynonym>(declarations[0], "p");
        require_value<AnyStmtSynonym>(declarations[1], "s");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "s");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedStmtStmtRel{"Follows*", untyped::UntypedStmtRef{untyped::UntypedSynonym{IDENT{"s"}}},
                                        untyped::UntypedStmtRef{Integer{"13"}}}};
        REQUIRE(such_that_clause == reference_clause);
    };

    SECTION("Query with stmt-ent relationship") {
        const auto query = R"(stmt s; Select s such that Uses(s, "v"))";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<AnyStmtSynonym>(declarations[0], "s");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "s");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedRefEntRel{"Uses", untyped::UntypedStmtEntRef{untyped::UntypedSynonym{IDENT{"s"}}},
                                      untyped::UntypedEntRef{QuotedIdent{"v"}}}};
        REQUIRE(such_that_clause == reference_clause);
    }

    SECTION("Query with ent-ent relationship for procedures") {
        const auto query = R"(procedure p; variable v; Select p such that Uses(p, "s"))";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 2);
        require_value<ProcSynonym>(declarations[0], "p");
        require_value<VarSynonym>(declarations[1], "v");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "p");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedSuchThatClause{
            untyped::UntypedRefEntRel{"Uses", untyped::UntypedStmtEntRef{untyped::UntypedSynonym{IDENT{"p"}}},
                                      untyped::UntypedEntRef{QuotedIdent{"s"}}}};
        REQUIRE(such_that_clause == reference_clause);
    }

    SECTION("Query with pattern clause") {
        const auto query = R"(        assign a;
Select a pattern a ( _ , _"count + 1"_))";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<AssignSynonym>(declarations[0], "a");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "a");

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

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<AssignSynonym>(declarations[0], "newa");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "newa");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[0]));
        const auto pattern_clause = std::get<untyped::UntypedPatternClause>(clauses[0]);
        const auto reference_clause =
            untyped::UntypedPatternClause{untyped::UntypedSynonym{IDENT{"newa"}}, QuotedIdent{"normSq"},
                                          ExpressionSpec{PartialMatch{"cenX cenX * "}}};
        REQUIRE(pattern_clause == reference_clause);
    }

    SECTION("Query with undefined synonym") {
        const auto query = "procedure p; Select v";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<ProcSynonym>(declarations[0], "p");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "v");

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

TEST_CASE("Test Parser - 'and' connectives for such that clauses") {
    const auto parser = untyped::DefaultUntypedParser{};

    SECTION("No 'and'") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(s, 13) such that Modifies(p, \"v\")";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 2);

        require_value<ProcSynonym>(declarations[0], "p");
        require_value<AnyStmtSynonym>(declarations[1], "s");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "s");

        REQUIRE(clauses.size() == 2);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause1 = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause1 = untyped::UntypedSuchThatClause{
            untyped::UntypedStmtStmtRel{"Follows*", untyped::UntypedStmtRef{untyped::UntypedSynonym{IDENT{"s"}}},
                                        untyped::UntypedStmtRef{Integer{"13"}}}};
        REQUIRE(such_that_clause1 == reference_clause1);

        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[1]));
        const auto such_that_clause2 = std::get<untyped::UntypedSuchThatClause>(clauses[1]);
        const auto reference_clause2 = untyped::UntypedSuchThatClause{
            untyped::UntypedRefEntRel{"Modifies", untyped::UntypedStmtEntRef{untyped::UntypedSynonym{IDENT{"p"}}},
                                      untyped::UntypedEntRef{QuotedIdent{"v"}}}};
        REQUIRE(such_that_clause2 == reference_clause2);
    }

    SECTION("One 'and'") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(s, 13) and Modifies(p, \"v\")";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 2);

        require_value<ProcSynonym>(declarations[0], "p");
        require_value<AnyStmtSynonym>(declarations[1], "s");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "s");

        REQUIRE(clauses.size() == 2);
        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[0]));
        const auto such_that_clause1 = std::get<untyped::UntypedSuchThatClause>(clauses[0]);
        const auto reference_clause1 = untyped::UntypedSuchThatClause{
            untyped::UntypedStmtStmtRel{"Follows*", untyped::UntypedStmtRef{untyped::UntypedSynonym{IDENT{"s"}}},
                                        untyped::UntypedStmtRef{Integer{"13"}}}};
        REQUIRE(such_that_clause1 == reference_clause1);

        REQUIRE(std::holds_alternative<untyped::UntypedSuchThatClause>(clauses[1]));
        const auto such_that_clause2 = std::get<untyped::UntypedSuchThatClause>(clauses[1]);
        const auto reference_clause2 = untyped::UntypedSuchThatClause{
            untyped::UntypedRefEntRel{"Modifies", untyped::UntypedStmtEntRef{untyped::UntypedSynonym{IDENT{"p"}}},
                                      untyped::UntypedEntRef{QuotedIdent{"v"}}}};
        REQUIRE(such_that_clause2 == reference_clause2);
    }

    SECTION("Syntax Error - 'and' with no relationship") {
        const auto query = " procedure p; stmt s; Select s such that Modifies(p, \"v\") and";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));
    }

    SECTION("Syntax Error - 'and' with no relationship 2") {
        const auto query = R"( procedure p; stmt s; Select s such that Modifies(p, "v") and Modifies(p, "v") and)";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));
    }
}

TEST_CASE("Test Parser - 'and' connectives for pattern clauses") {
    const auto parser = untyped::DefaultUntypedParser{};

    SECTION("No 'and'") {
        const auto query =
            R"(assign newa;Select newa pattern newa ( "normSq" , _"cenX * cenX"_) pattern newa ( "normSq" , _"cenX"_))";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<AssignSynonym>(declarations[0], "newa");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "newa");

        REQUIRE(clauses.size() == 2);
        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[0]));
        const auto pattern_clause = std::get<untyped::UntypedPatternClause>(clauses[0]);
        const auto reference_clause =
            untyped::UntypedPatternClause{untyped::UntypedSynonym{IDENT{"newa"}}, QuotedIdent{"normSq"},
                                          ExpressionSpec{PartialMatch{"cenX cenX * "}}};
        REQUIRE(pattern_clause == reference_clause);

        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[1]));
        const auto pattern_clause2 = std::get<untyped::UntypedPatternClause>(clauses[1]);
        const auto reference_clause2 = untyped::UntypedPatternClause{
            untyped::UntypedSynonym{IDENT{"newa"}}, QuotedIdent{"normSq"}, ExpressionSpec{PartialMatch{"cenX "}}};
        REQUIRE(pattern_clause2 == reference_clause2);
    }

    SECTION("One 'and'") {
        const auto query =
            R"(assign newa;Select newa pattern newa ( "normSq" , _"cenX * cenX"_) and newa ( "normSq" , _"cenX"_))";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<AssignSynonym>(declarations[0], "newa");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "newa");

        REQUIRE(clauses.size() == 2);
        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[0]));
        const auto pattern_clause = std::get<untyped::UntypedPatternClause>(clauses[0]);
        const auto reference_clause =
            untyped::UntypedPatternClause{untyped::UntypedSynonym{IDENT{"newa"}}, QuotedIdent{"normSq"},
                                          ExpressionSpec{PartialMatch{"cenX cenX * "}}};
        REQUIRE(pattern_clause == reference_clause);

        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[1]));
        const auto pattern_clause2 = std::get<untyped::UntypedPatternClause>(clauses[1]);
        const auto reference_clause2 = untyped::UntypedPatternClause{
            untyped::UntypedSynonym{IDENT{"newa"}}, QuotedIdent{"normSq"}, ExpressionSpec{PartialMatch{"cenX "}}};
        REQUIRE(pattern_clause2 == reference_clause2);
    }

    SECTION("Syntax Error - 'and' with no pattern") {
        const auto query = R"(assign a; Select a pattern a ( _ , _"count + 1"_) and)";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));
    }

    SECTION("Syntax Error - 'and' with no pattern 2") {
        const auto query = R"(assign a; Select a pattern a ( _ , _"count + 1"_) and a ( _ , _"count + 1"_) and)";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));
    }
}

TEST_CASE("Test Parser - pattern clause with 3 arg") {
    const auto parser = untyped::DefaultUntypedParser{};

    SECTION("Test Parser - pattern if") {
        const auto query = R"(if a; variable v; Select a pattern a ( v , _, _))";
        const auto output = parser.parse(query);

        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 2);
        require_value<IfSynonym>(declarations[0], "a");
        require_value<VarSynonym>(declarations[1], "v");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "a");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedPatternClause>(clauses[0]));
        const auto pattern_clause = std::get<untyped::UntypedPatternClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedPatternClause{untyped::UntypedSynonym{IDENT{"a"}},
                                                                    untyped::UntypedSynonym{IDENT{"v"}}, WildCard{}};
        REQUIRE(pattern_clause == reference_clause);
    }

    SECTION("Test Parser - pattern 3 args Syntax Error") {
        const auto query = R"(while a; variable v; Select a pattern a ( _ , _, v))";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<SyntaxError>(output));

        const auto query2 = R"(while a; variable v; Select a pattern a ( _ , v, _))";
        const auto output2 = parser.parse(query2);
        REQUIRE(std::holds_alternative<SyntaxError>(output2));
    }
}

TEST_CASE("Test Parser - Basic With clause") {
    const auto parser = untyped::DefaultUntypedParser{};

    SECTION("Test Parser - with stmt#") {
        const auto query = R"(variable v; Select v with v.stmt# = 1)";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<VarSynonym>(declarations[0], "v");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "v");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedWithClause>(clauses[0]));
        const auto with_clause = std::get<untyped::UntypedWithClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedWithClause{
            untyped::UntypedAttrRef{untyped::UntypedSynonym{IDENT{"v"}}, StmtNum{}}, Integer{"1"}};
        REQUIRE(with_clause == reference_clause);
    }

    SECTION("Test Parser - with value") {
        const auto query = R"(variable v; Select v with v.value = "x")";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<VarSynonym>(declarations[0], "v");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "v");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedWithClause>(clauses[0]));
        const auto with_clause = std::get<untyped::UntypedWithClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedWithClause{
            untyped::UntypedAttrRef{untyped::UntypedSynonym{IDENT{"v"}}, Value{}}, QuotedIdent{"x"}};
        REQUIRE(with_clause == reference_clause);
    }

    SECTION("Test Parser - with procName") {
        const auto query = R"(procedure p; Select p with p.procName = "x")";
        const auto output = parser.parse(query);
        REQUIRE(std::holds_alternative<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output));
        const auto& [declarations, untyped] =
            std::get<std::tuple<Synonyms, untyped::DefaultUntypedParser::UntypedQueryType>>(output);

        REQUIRE(declarations.size() == 1);
        require_value<ProcSynonym>(declarations[0], "p");

        const auto& [references, clauses] = untyped;
        REQUIRE(std::holds_alternative<UntypedVector>(references));
        const auto& reference = std::get<UntypedVector>(references);
        require_value<untyped::UntypedSynonym>(reference, "p");

        REQUIRE(clauses.size() == 1);
        REQUIRE(std::holds_alternative<untyped::UntypedWithClause>(clauses[0]));
        const auto with_clause = std::get<untyped::UntypedWithClause>(clauses[0]);
        const auto reference_clause = untyped::UntypedWithClause{
            untyped::UntypedAttrRef{untyped::UntypedSynonym{IDENT{"p"}}, ProcName{}}, QuotedIdent{"x"}};
        REQUIRE(with_clause == reference_clause);

        SECTION("Transitive relationships") {
            const auto query = "variable v; select v such that Parent *(v, 1)";
            const auto output = parser.parse(query);
            REQUIRE(std::holds_alternative<SyntaxError>(output)); // Parent* instead of Parent *

            const auto query2 = "variable v; select v such that Follows *(v, 1)";
            const auto output2 = parser.parse(query);
            REQUIRE(std::holds_alternative<SyntaxError>(output2)); // Follows* instead of Follows *
        }
    }
}