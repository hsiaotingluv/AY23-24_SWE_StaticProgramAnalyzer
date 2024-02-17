#include "catch.hpp"

#include "utils.hpp"

#include "qps/parser/expression_parser.hpp"
#include "qps/qps.hpp"
#include "qps/tokeniser/tokeniser.hpp"

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <iterator>
#include <memory>
#include <optional>

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
        require_value<ProcSynonym>(result[0], "p");
        REQUIRE(std::distance(rest, tokens.end()) == 0);
    }

    SECTION("Declaration with multiple synonyms") {
        const auto query = "procedure p, q, r; Select p";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 3);
        require_value<ProcSynonym>(result[0], "p");
        require_value<ProcSynonym>(result[1], "q");
        require_value<ProcSynonym>(result[2], "r");
        REQUIRE(std::distance(rest, tokens.end()) == 2);
    }

    SECTION("Multiple declarations") {
        const auto query = "procedure p ; variable v; Select p";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 2);
        require_value<ProcSynonym>(result[0], "p");
        require_value<VarSynonym>(result[1], "v");
        REQUIRE(std::distance(rest, tokens.end()) == 2);
    }

    SECTION("Declaration with invalid keyword") {
        const auto query = "proc p;";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = parse_declarations(tokens.begin(), tokens.end());

        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test QPS - Basic Functionality") {
    const auto qps = QueryProcessingSystem{};

#ifndef MILESTONE1
    SECTION("Query with stmt-stmt relationship") {
        const auto query = " procedure p; stmt s; Select s such that Follows(s, 13) such that Follows*(13, s) such "
                           "that Parent (s, 12) such that Parent*(12, s) ";
        const auto output = parser.parse(query);

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        REQUIRE(std::holds_alternative<ProcSynonym>(result.declared[0]));
        REQUIRE(std::get<ProcSynonym>(result.declared[0]).get_name() == "p");

        REQUIRE(std::holds_alternative<AnyStmtSynonymynonym>(result.declared[1]));
        REQUIRE(std::get<AnyStmtSynonymynonym>(result.declared[1]).get_name() == "s");

        REQUIRE(std::holds_alternative<AnyStmtSynonymynonym>(result.reference));
        REQUIRE(std::get<AnyStmtSynonymynonym>(result.reference).get_name() == "s");
    }
#else
    SECTION("Query with stmt-stmt relationship") {
        const auto query = " procedure p; stmt s; Select s such that Follows(s, 13)";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        require_value<ProcSynonym>(result.declared[0], "p");
        require_value<AnyStmtSynonym>(result.declared[1], "s");
        require_value<AnyStmtSynonym>(result.reference, "s");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            Follows{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{13}});
    }
#endif
    SECTION("Query with stmt-ent relationship") {

#ifndef MILESTONE1
        const auto query =
            R"(stmt s; Select s such that Uses(s, "v") such that Modifies(s, "q") such that Modifies("x", "y"))";
        const auto result = parser.parse(query);

        REQUIRE(result.has_value());
#else
        const auto query = R"(stmt s; Select s such that Uses(s, "v"))";
        const auto result = to_query(qps.parse(query));

        REQUIRE(result.has_value());
        REQUIRE(result->declared.size() == 1);
        require_value<AnyStmtSynonym>(result->declared[0], "s");
        require_value<AnyStmtSynonym>(result->reference, "s");

        REQUIRE(result->clauses.size() == 1);
        const auto reference_clause =
            std::make_shared<SuchThatClause>(UsesS{std::make_shared<AnyStmtSynonym>(IDENT{"s"}), QuotedIdent{"v"}});
#endif
    }

    SECTION("Query with stmt-ent relationship") {
        const auto query = R"(procedure p; variable v; Select v such that Uses(1, v))";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        require_value<ProcSynonym>(result.declared[0], "p");
        require_value<VarSynonym>(result.declared[1], "v");
        require_value<VarSynonym>(result.reference, "v");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause =
            std::make_shared<SuchThatClause>(UsesS{Integer{1}, std::make_shared<VarSynonym>(IDENT{"v"})});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

#ifndef MILESTONE1
    SECTION("Query with ent-ent relationship for procedures") {
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
#endif

    SECTION("Query with pattern clause") {
        const auto query = R"(        assign a;
Select a pattern a ( _ , _"count + 1"_))";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);
        require_value<AssignSynonym>(result.declared[0], "a");
        require_value<AssignSynonym>(result.reference, "a");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<PatternClause>(std::make_shared<AssignSynonym>(IDENT{"a"}),
                                                                      WildCard{}, PartialMatch{"count 1 + "});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with pattern clause 2") {
        const auto query = R"(assign newa;Select newa pattern newa ( "normSq" , _"cenX * cenX"_))";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);

        require_value<AssignSynonym>(result.declared[0], "newa");
        require_value<AssignSynonym>(result.reference, "newa");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<PatternClause>(
            std::make_shared<AssignSynonym>(IDENT{"newa"}), QuotedIdent{"normSq"}, PartialMatch{"cenX cenX * "});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

#ifndef MILESTONE1
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
        const auto reference_clause = std::make_shared<SuchThatClause>(
            ParentT{StmtSynonym{WhileSynonym{IDENT{"w"}}}, StmtSynonym{AssignSynonym{IDENT{"a"}}}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);

        const auto reference_clause2 =
            std::make_shared<PatternClause>(AssignSynonym{IDENT{"a"}}, QuotedIdent{"count"}, ExpressionSpec{"_"});
        REQUIRE(*(result.clauses[1]) == *reference_clause2);
    }
#endif
}

TEST_CASE("Test QPS - Syntax") {
    const auto qps = QueryProcessingSystem{};
    SECTION("Query with invalid keyword") {
        const auto query = "proc p;";
        const auto output = qps.parse(query);

        REQUIRE(is_syntax_error(output));
    }

    SECTION("Query with invalid keyword") {
        const auto query = "Procedure p; ";
        const auto output = qps.parse(query);

        REQUIRE(is_syntax_error(output));
    }

    SECTION("Query with no clause") {
        // select-cl : declaration* 'Select' synonym [ suchthat-cl ] [ pattern-cl ]
        const auto query = "procedure p;";
        const auto output = qps.parse(query);

        REQUIRE(is_syntax_error(output));
    }

    SECTION("Query with no design entity") {
        const auto query = "stmt; Select s";
        const auto output = qps.parse(query);

        REQUIRE(is_syntax_error(output));
    }

#ifdef MILESTONE1
    SECTION("Query with semantic issues and syntactically wrong expression") {
        const auto query = R"(procedure p; Select v pattern a (_, "count"))";
        const auto output = qps.parse(query);

        // expression-spec : '_' '"' factor '"' '_' | '_'
        REQUIRE(is_syntax_error(output));
    }
#endif
}

TEST_CASE("Test QPS - Semantics") {
    const auto qps = QueryProcessingSystem{};
    SECTION("Query with undefined synonym") {
        const auto query = "procedure p; Select v";
        const auto result = qps.parse(query);

        REQUIRE(is_semantic_error(result));

        const auto query2 = R"(procedure p; Select v pattern a (_, _"count"_))";
        const auto output2 = qps.parse(query2);

        // expression-spec : '_' '"' factor '"' '_' | '_'
        REQUIRE(is_semantic_error(output2));
    }

    SECTION("Query with semantic issues") {

        const auto query2 = "procedure p; Select p such that Modifies(1, p)";
        const auto output2 = qps.parse(query2);
        REQUIRE(is_semantic_error(output2));
    }

    SECTION("Modifies cannot start with wildcard") {
        const auto query = "procedure p; variable v; Select p such that Modifies(_, v)";
        const auto output = qps.parse(query);

        REQUIRE(is_semantic_error(output));
    }

    SECTION("Uses cannot start with wildcard") {
        const auto query = "procedure p; variable v; Select p such that Uses(_, v)";
        const auto output = qps.parse(query);

        REQUIRE(is_semantic_error(output));
    }

    SECTION("Query with multiple declarations of the same synonym") {
        const auto query = "procedure p, p; Select p";
        const auto output = qps.parse(query);

        REQUIRE(is_semantic_error(output));
    }

    SECTION("Query with multiple declarations of the same name") {
        const auto query = "procedure p; variable p; Select p";
        const auto output = qps.parse(query);

        REQUIRE(is_semantic_error(output));
    }
}

TEST_CASE("Test QPS - long queries") {
    const auto qps = QueryProcessingSystem{};

    SECTION("Long Query") {
        const auto query = "stmt s, s1, s2; assign a, a1; while w; if ifs; variable v; procedure p, q; constant c; "
                           "print pn; read re;  Select p such that Parent (7, 20)";
        const auto output = to_query(qps.parse(query));
        REQUIRE(output.has_value());
    }
}