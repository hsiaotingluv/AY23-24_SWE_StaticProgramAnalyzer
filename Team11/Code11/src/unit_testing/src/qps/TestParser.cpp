#include "catch.hpp"

#include "qps/parser/parser.hpp"
#include "qps/tokeniser/tokeniser.hpp"

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <iterator>
#include <memory>
#include <variant>

using namespace qps;

TEST_CASE("Test Declaration Parser") {
    const auto runner = tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

    SECTION("Declaration with one synonym") {
        const auto query = "procedure p;";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 1);
        REQUIRE(std::holds_alternative<ProcSynonym>(result[0]));
        REQUIRE(std::get<ProcSynonym>(result[0]).get_name() == "p");
        REQUIRE(std::distance(rest, tokens.end()) == 0);
    }

    SECTION("Declaration with multiple synonyms") {
        const auto query = "procedure p, q, r; Select p";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 3);
        REQUIRE(std::holds_alternative<ProcSynonym>(result[0]));
        REQUIRE(std::get<ProcSynonym>(result[0]).get_name() == "p");
        REQUIRE(std::holds_alternative<ProcSynonym>(result[1]));
        REQUIRE(std::get<ProcSynonym>(result[1]).get_name() == "q");
        REQUIRE(std::holds_alternative<ProcSynonym>(result[2]));
        REQUIRE(std::get<ProcSynonym>(result[2]).get_name() == "r");
        REQUIRE(std::distance(rest, tokens.end()) == 2);
    }

    SECTION("Multiple declarations") {
        const auto query = "procedure p ; variable v; Select p";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 2);
        REQUIRE(std::holds_alternative<ProcSynonym>(result[0]));
        REQUIRE(std::get<ProcSynonym>(result[0]).get_name() == "p");
        REQUIRE(std::holds_alternative<VarSynonym>(result[1]));
        REQUIRE(std::get<VarSynonym>(result[1]).get_name() == "v");
        REQUIRE(std::distance(rest, tokens.end()) == 2);
    }

    SECTION("Declaration with invalid keyword") {
        const auto query = "proc p;";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Parser") {
    const auto parser = QueryProcessingSystemParser{};
    SECTION("Query with stmt-stmt relationship") {
        const auto query = " procedure p; stmt s; Select s such that Follows(s, 13) such that Follows*(13, s) such "
                           "that Parent (s, 12) such that Parent*(12, s) ";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        REQUIRE(std::holds_alternative<ProcSynonym>(result.declared[0]));
        REQUIRE(std::get<ProcSynonym>(result.declared[0]).get_name() == "p");

        REQUIRE(std::holds_alternative<UntypedStmtSynonym>(result.declared[1]));
        REQUIRE(std::get<UntypedStmtSynonym>(result.declared[1]).get_name() == "s");

        REQUIRE(std::holds_alternative<UntypedStmtSynonym>(result.reference));
        REQUIRE(std::get<UntypedStmtSynonym>(result.reference).get_name() == "s");
    }

    SECTION("Query with stmt-ent relationship") {
        const auto query =
            R"(stmt s; Select s such that Uses(s, "v") such that Modifies(s, "q") such that Modifies("x", "y"))";
        const auto result = parser.parse(query);

        REQUIRE(result.has_value());
    }

    SECTION("Query with ent-ent relationship") {
        const auto query = R"(procedure p; variable v; Select v such that Uses("s", v))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        REQUIRE(std::holds_alternative<ProcSynonym>(result.declared[0]));
        REQUIRE(std::get<ProcSynonym>(result.declared[0]).get_name() == "p");

        REQUIRE(std::holds_alternative<VarSynonym>(result.declared[1]));
        REQUIRE(std::get<VarSynonym>(result.declared[1]).get_name() == "v");

        REQUIRE(std::holds_alternative<VarSynonym>(result.reference));
        REQUIRE(std::get<VarSynonym>(result.reference).get_name() == "v");

        REQUIRE(result.clauses.size() == 1);
        const std::shared_ptr<Clause> reference_clause =
            std::make_shared<SuchThatClause>(UsesP{QuotedIdent{"s"}, VarSynonym{IDENT{"v"}}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with ent-ent relationship 2") {
        const auto query = R"(procedure p; variable v; Select p such that Uses(p, "s"))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        REQUIRE(std::holds_alternative<ProcSynonym>(result.declared[0]));
        REQUIRE(std::get<ProcSynonym>(result.declared[0]).get_name() == "p");

        REQUIRE(std::holds_alternative<VarSynonym>(result.declared[1]));
        REQUIRE(std::get<VarSynonym>(result.declared[1]).get_name() == "v");

        REQUIRE(std::holds_alternative<ProcSynonym>(result.reference));
        REQUIRE(std::get<ProcSynonym>(result.reference).get_name() == "p");
    }

    SECTION("Query with pattern clause") {
        const auto query = R"(        assign a;
Select a pattern a ( _ , "count + 1"))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);
        REQUIRE(std::holds_alternative<AssignSynonym>(result.declared[0]));
        REQUIRE(std::get<AssignSynonym>(result.declared[0]).get_name() == "a");

        REQUIRE(std::holds_alternative<AssignSynonym>(result.reference));
        REQUIRE(std::get<AssignSynonym>(result.reference).get_name() == "a");

        REQUIRE(result.clauses.size() == 1);
        const std::shared_ptr<Clause> reference_clause =
            std::make_shared<PatternClause>(AssignSynonym{IDENT{"a"}}, WildCard{}, ExpressionSpec{"((count)+(1))"});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with pattern clause 2") {
        const auto query = R"(assign newa;Select newa pattern newa ( "normSq" , _"cenX * cenX"_))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);
        REQUIRE(std::holds_alternative<AssignSynonym>(result.declared[0]));
        REQUIRE(std::get<AssignSynonym>(result.declared[0]).get_name() == "newa");

        REQUIRE(std::holds_alternative<AssignSynonym>(result.reference));
        REQUIRE(std::get<AssignSynonym>(result.reference).get_name() == "newa");

        REQUIRE(result.clauses.size() == 1);
        const std::shared_ptr<Clause> reference_clause = std::make_shared<PatternClause>(
            AssignSynonym{IDENT{"newa"}}, QuotedIdent{"normSq"}, ExpressionSpec{"_((cenX)*(cenX))_"});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with such that clause and pattern clause") {
        const auto query = R"(assign a; while w; Select w such that Parent* (w, a) pattern a ("count", _))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        REQUIRE(std::holds_alternative<AssignSynonym>(result.declared[0]));
        REQUIRE(std::get<AssignSynonym>(result.declared[0]).get_name() == "a");
        REQUIRE(std::holds_alternative<WhileSynonym>(result.declared[1]));
        REQUIRE(std::get<WhileSynonym>(result.declared[1]).get_name() == "w");

        REQUIRE(std::holds_alternative<WhileSynonym>(result.reference));
        REQUIRE(std::get<WhileSynonym>(result.reference).get_name() == "w");

        REQUIRE(result.clauses.size() == 2);
        const std::shared_ptr<Clause> reference_clause = std::make_shared<SuchThatClause>(
            ParentT{StmtSynonym{WhileSynonym{IDENT{"w"}}}, StmtSynonym{AssignSynonym{IDENT{"a"}}}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);

        const std::shared_ptr<Clause> reference_clause2 =
            std::make_shared<PatternClause>(AssignSynonym{IDENT{"a"}}, QuotedIdent{"count"}, ExpressionSpec{"_"});
        REQUIRE(*(result.clauses[1]) == *reference_clause2);
    }

    SECTION("Query with undefined synonym") {
        const auto query = "procedure p; Select v";
        const auto result = parser.parse(query);

        REQUIRE(!result.has_value());
    }

    SECTION("Query with semantic issues") {
        const auto query = R"(procedure p; variable v; Select p such that Uses("s", p))";
        const auto output = parser.parse(query);

        REQUIRE(!output.has_value());

        const auto query2 = "procedure p; Select p such that Modifies(1, p)";
        const auto output2 = parser.parse(query2);
        REQUIRE(!output2.has_value());

        const auto query3 = "variable v; Select v such that Uses(v, 1)";
        const auto output3 = parser.parse(query3);
        REQUIRE(!output3.has_value());
    }

    SECTION("Modifies cannot start with wildcard") {
        const auto query = "procedure p; variable v; Select p such that Modifies(_, v)";
        const auto output = parser.parse(query);

        REQUIRE(!output.has_value());
    }

    SECTION("Uses cannot start with wildcard") {
        const auto query = "procedure p; variable v; Select p such that Uses(_, v)";
        const auto output = parser.parse(query);

        REQUIRE(!output.has_value());
    }
}