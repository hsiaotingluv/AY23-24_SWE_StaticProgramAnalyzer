#include "catch.hpp"

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/parser.hpp"
#include "qps/tokeniser/tokeniser.hpp"

#include <iterator>
#include <variant>

using namespace qps;

TEST_CASE("Test Declaration Parser") {
    const auto runner = tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

    SECTION("Declaration with one synonym") {
        const auto query = "procedure p;";
        auto tokens = runner.apply_tokeniser(query);
        const auto output = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 1);
        REQUIRE(std::holds_alternative<ProcSynonym>(result[0]));
        REQUIRE(std::get<ProcSynonym>(result[0]).name == "p");
        REQUIRE(std::distance(rest, tokens.end()) == 0);
    }

    SECTION("Declaration with multiple synonyms") {
        const auto query = "procedure p, q, r; Select p";
        auto tokens = runner.apply_tokeniser(query);
        const auto output = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 3);
        REQUIRE(std::holds_alternative<ProcSynonym>(result[0]));
        REQUIRE(std::get<ProcSynonym>(result[0]).name == "p");
        REQUIRE(std::holds_alternative<ProcSynonym>(result[1]));
        REQUIRE(std::get<ProcSynonym>(result[1]).name == "q");
        REQUIRE(std::holds_alternative<ProcSynonym>(result[2]));
        REQUIRE(std::get<ProcSynonym>(result[2]).name == "r");
        REQUIRE(std::distance(rest, tokens.end()) == 2);
    }

    SECTION("Multiple declarations") {
        const auto query = "procedure p ; variable v; Select p";
        auto tokens = runner.apply_tokeniser(query);
        const auto output = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 2);
        REQUIRE(std::holds_alternative<ProcSynonym>(result[0]));
        REQUIRE(std::get<ProcSynonym>(result[0]).name == "p");
        REQUIRE(std::holds_alternative<VarSynonym>(result[1]));
        REQUIRE(std::get<VarSynonym>(result[1]).name == "v");
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
        REQUIRE(std::get<ProcSynonym>(result.declared[0]).name == "p");

        REQUIRE(std::holds_alternative<RawStmtSynonym>(result.declared[1]));
        REQUIRE(std::get<RawStmtSynonym>(result.declared[1]).name == "s");

        REQUIRE(std::holds_alternative<RawStmtSynonym>(result.reference));
        REQUIRE(std::get<RawStmtSynonym>(result.reference).name == "s");
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
        REQUIRE(std::get<ProcSynonym>(result.declared[0]).name == "p");

        REQUIRE(std::holds_alternative<VarSynonym>(result.declared[1]));
        REQUIRE(std::get<VarSynonym>(result.declared[1]).name == "v");

        REQUIRE(std::holds_alternative<VarSynonym>(result.reference));
        REQUIRE(std::get<VarSynonym>(result.reference).name == "v");

        REQUIRE(result.clauses.size() == 1);
        REQUIRE(std::holds_alternative<SuchThatClause>(result.clauses[0]));
        const auto& clause = std::get<SuchThatClause>(result.clauses[0]);
        REQUIRE(std::holds_alternative<UsesP>(clause.rel_ref));
        const auto& rel_ref = std::get<UsesP>(clause.rel_ref);
        REQUIRE(std::holds_alternative<QuotedIdent>(rel_ref.ent1));
        REQUIRE(std::get<QuotedIdent>(rel_ref.ent1) == "s");

        REQUIRE(std::holds_alternative<Synonym>(rel_ref.ent2));
        const auto& syn = std::get<Synonym>(rel_ref.ent2);
        REQUIRE(std::holds_alternative<VarSynonym>(syn));
        REQUIRE(std::get<VarSynonym>(syn).name == "v");
    }

    SECTION("Query with ent-ent relationship 2") {
        const auto query = R"(procedure p; variable v; Select p such that Uses(p, "s"))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        REQUIRE(std::holds_alternative<ProcSynonym>(result.declared[0]));
        REQUIRE(std::get<ProcSynonym>(result.declared[0]).name == "p");

        REQUIRE(std::holds_alternative<VarSynonym>(result.declared[1]));
        REQUIRE(std::get<VarSynonym>(result.declared[1]).name == "v");

        REQUIRE(std::holds_alternative<ProcSynonym>(result.reference));
        REQUIRE(std::get<ProcSynonym>(result.reference).name == "p");
    }

    SECTION("Query with pattern clause") {
        const auto query = R"(        assign a;
Select a pattern a ( _ , "count + 1"))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);
        REQUIRE(std::holds_alternative<AssignSynonym>(result.declared[0]));
        REQUIRE(std::get<AssignSynonym>(result.declared[0]).name == "a");

        REQUIRE(std::holds_alternative<AssignSynonym>(result.reference));
        REQUIRE(std::get<AssignSynonym>(result.reference).name == "a");

        REQUIRE(result.clauses.size() == 1);
        REQUIRE(std::holds_alternative<PatternClause>(result.clauses[0]));
        const auto& clause = std::get<PatternClause>(result.clauses[0]);
        REQUIRE(clause.assign_synonym.name == "a");
        REQUIRE(std::holds_alternative<WildCard>(clause.ent_ref));
        REQUIRE(clause.expression_spec.value == "((count)+(1))");
    }

    SECTION("Query with pattern clause 2") {
        const auto query = R"(assign newa;Select newa pattern newa ( "normSq" , _"cenX * cenX"_))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);
        REQUIRE(std::holds_alternative<AssignSynonym>(result.declared[0]));
        REQUIRE(std::get<AssignSynonym>(result.declared[0]).name == "newa");

        REQUIRE(std::holds_alternative<AssignSynonym>(result.reference));
        REQUIRE(std::get<AssignSynonym>(result.reference).name == "newa");

        REQUIRE(result.clauses.size() == 1);
        REQUIRE(std::holds_alternative<PatternClause>(result.clauses[0]));
        const auto& clause = std::get<PatternClause>(result.clauses[0]);
        REQUIRE(clause.assign_synonym.name == "newa");
        REQUIRE(std::holds_alternative<QuotedIdent>(clause.ent_ref));
        REQUIRE(std::get<QuotedIdent>(clause.ent_ref) == "normSq");
        REQUIRE(clause.expression_spec.value == "_((cenX)*(cenX))_");
    }

    SECTION("Query with such that clause and pattern clause") {
        const auto query = R"(assign a; while w; Select w such that Parent* (w, a) pattern a ("count", _))";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        REQUIRE(std::holds_alternative<AssignSynonym>(result.declared[0]));
        REQUIRE(std::get<AssignSynonym>(result.declared[0]).name == "a");
        REQUIRE(std::holds_alternative<WhileSynonym>(result.declared[1]));
        REQUIRE(std::get<WhileSynonym>(result.declared[1]).name == "w");

        REQUIRE(std::holds_alternative<WhileSynonym>(result.reference));
        REQUIRE(std::get<WhileSynonym>(result.reference).name == "w");

        REQUIRE(result.clauses.size() == 2);
        REQUIRE(std::holds_alternative<SuchThatClause>(result.clauses[0]));
        const auto& such_that_clause = std::get<SuchThatClause>(result.clauses[0]);
        REQUIRE(std::holds_alternative<ParentT>(such_that_clause.rel_ref));
        const auto& rel_ref = std::get<ParentT>(such_that_clause.rel_ref);
        REQUIRE(std::holds_alternative<StmtSynonym>(rel_ref.stmt1));
        const auto syn1 = std::get<StmtSynonym>(rel_ref.stmt1);
        REQUIRE(std::holds_alternative<WhileSynonym>(syn1));
        REQUIRE(std::get<WhileSynonym>(syn1).name == "w");
        REQUIRE(std::holds_alternative<StmtSynonym>(rel_ref.stmt2));
        const auto syn2 = std::get<StmtSynonym>(rel_ref.stmt2);
        REQUIRE(std::holds_alternative<AssignSynonym>(syn2));
        REQUIRE(std::get<AssignSynonym>(syn2).name == "a");

        REQUIRE(std::holds_alternative<PatternClause>(result.clauses[1]));
        const auto& pattern_clause = std::get<PatternClause>(result.clauses[1]);
        REQUIRE(pattern_clause.assign_synonym.name == "a");
        REQUIRE(std::holds_alternative<QuotedIdent>(pattern_clause.ent_ref));
        REQUIRE(std::get<QuotedIdent>(pattern_clause.ent_ref) == "count");
        REQUIRE(pattern_clause.expression_spec.value == "_");
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
    }
}