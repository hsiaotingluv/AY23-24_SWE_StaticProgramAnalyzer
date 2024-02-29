#include "catch.hpp"

#include "qps/parser/untyped/untyped_parser.hpp"
#include "utils.hpp"

#include "qps/parser.hpp"
#include "qps/parser/expression_parser.hpp"
#include "qps/tokeniser/tokeniser.hpp"

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <iterator>
#include <memory>
#include <optional>
#include <variant>

using namespace qps;

TEST_CASE("Test Declaration Parser") {
    const auto runner = tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

    SECTION("Declaration with one synonym") {
        const auto query = "procedure p;";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output =
            untyped::detail::parse_declarations(tokens.begin(), tokens.end(), untyped::DefaultSupportedSynonyms{});

        REQUIRE(output.has_value());
        const auto& [result, rest] = output.value();

        REQUIRE(result.size() == 1);
        require_value<ProcSynonym>(result[0], "p");
        REQUIRE(std::distance(rest, tokens.end()) == 0);
    }

    SECTION("Declaration with multiple synonyms") {
        const auto query = "procedure p, q, r; Select p";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output =
            untyped::detail::parse_declarations(tokens.begin(), tokens.end(), untyped::DefaultSupportedSynonyms{});

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
        const auto output =
            untyped::detail::parse_declarations(tokens.begin(), tokens.end(), untyped::DefaultSupportedSynonyms{});

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
        const auto result =
            untyped::detail::parse_declarations(tokens.begin(), tokens.end(), untyped::DefaultSupportedSynonyms{});

        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test QPS - Basic Functionality") {
    const auto qps = qps::DefaultParser{};
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
            Follows{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{"13"}});
    }

    SECTION("Query with stmt-stmt relationship - Follows*") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(s, 13)";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        require_value<ProcSynonym>(result.declared[0], "p");
        require_value<AnyStmtSynonym>(result.declared[1], "s");
        require_value<AnyStmtSynonym>(result.reference, "s");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            FollowsT{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{"13"}});
    }

    SECTION("Query with stmt-stmt relationship - Parent*") {
        const auto query = " procedure p; stmt s; Select s such that Parent*(s, 13)";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        require_value<ProcSynonym>(result.declared[0], "p");
        require_value<AnyStmtSynonym>(result.declared[1], "s");
        require_value<AnyStmtSynonym>(result.reference, "s");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            ParentT{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{"13"}});
    }

    SECTION("Query with stmt-ent relationship") {

        const auto query = R"(stmt s; Select s such that Uses(s, "v"))";
        const auto result = to_query(qps.parse(query));

        REQUIRE(result.has_value());
        REQUIRE(result->declared.size() == 1);
        require_value<AnyStmtSynonym>(result->declared[0], "s");
        require_value<AnyStmtSynonym>(result->reference, "s");

        REQUIRE(result->clauses.size() == 1);
        const auto reference_clause =
            std::make_shared<SuchThatClause>(UsesS{std::make_shared<AnyStmtSynonym>(IDENT{"s"}), QuotedIdent{"v"}});
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
            std::make_shared<SuchThatClause>(UsesS{Integer{"1"}, std::make_shared<VarSynonym>(IDENT{"v"})});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

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
        const auto reference_clause = std::make_shared<PatternAssignClause>(std::make_shared<AssignSynonym>(IDENT{"a"}),
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
        const auto reference_clause = std::make_shared<PatternAssignClause>(
            std::make_shared<AssignSynonym>(IDENT{"newa"}), QuotedIdent{"normSq"}, PartialMatch{"cenX cenX * "});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }
}

TEST_CASE("Test QPS - Syntax") {
    const auto qps = qps::DefaultParser{};
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
}

TEST_CASE("Test QPS - Semantics") {
    const auto qps = qps::DefaultParser{};
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
    const auto qps = qps::DefaultParser{};

    SECTION("Long Query") {
        const auto query = "stmt s, s1, s2; assign a, a1; while w; if ifs; variable v; procedure p, q; constant c; "
                           "print pn; read re;  Select p such that Parent (7, 20)";
        const auto output = to_query(qps.parse(query));
        REQUIRE(output.has_value());
    }
}

TEST_CASE("Test QPS - Select multiple synonyms") {
    const auto qps = qps::DefaultParser{};

    SECTION("Query with stmt-stmt relationship - 0 synonyms") {
        const auto query = " procedure p; stmt s; Select <> such that Follows(s, 13)";
        const auto output = to_query(qps.parse(query));

        REQUIRE(!output.has_value());
    }

    SECTION("Query with stmt-stmt relationship - 1 synonyms") {
        const auto query = " procedure p; stmt s; Select <s> such that Follows(s, 13)";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        require_value<ProcSynonym>(result.declared[0], "p");
        require_value<AnyStmtSynonym>(result.declared[1], "s");

        require_value<AnyStmtSynonym>(result.reference, "s");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            Follows{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{"13"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with stmt-stmt relationship - 2 synonyms") {
        const auto query = " procedure p; stmt s; Select <s, p> such that Follows(s, 13)";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        require_value<ProcSynonym>(result.declared[0], "p");
        require_value<AnyStmtSynonym>(result.declared[1], "s");

        require_value<AnyStmtSynonym>(result.reference, "s");
        require_value<ProcSynonym>(result.reference, "p");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            Follows{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{"13"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }
}
TEST_CASE("Test QPS - 'and' connectives") {
    const auto qps = qps::DefaultParser{};

    SECTION("'FollowsT' and 'ModifiesP'") {
        const auto query = " procedure p; stmt s; Select s such that Follows*(s, 13) and Modifies(p, \"v\")";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        require_value<ProcSynonym>(result.declared[0], "p");
        require_value<AnyStmtSynonym>(result.declared[1], "s");

        require_value<AnyStmtSynonym>(result.reference, "s");

        REQUIRE(result.clauses.size() == 2);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            FollowsT{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{"13"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);

        const auto reference_clause2 = std::make_shared<SuchThatClause>(
            ModifiesP{ProcedureRefNoWildcard{std::make_shared<ProcSynonym>(IDENT{"p"})}, QuotedIdent{"v"}});
        REQUIRE(*(result.clauses[1]) == *reference_clause2);
    }

    SECTION("'pattern A and B' ") {
        const auto query =
            R"(assign newa;Select newa pattern newa ( "normSq" , _"cenX * cenX"_) and newa ( "normSq" , _"cenX"_))";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);
        require_value<AssignSynonym>(result.declared[0], "newa");

        require_value<AssignSynonym>(result.reference, "newa");

        REQUIRE(result.clauses.size() == 2);
        const auto reference_clause = std::make_shared<PatternAssignClause>(
            std::make_shared<AssignSynonym>(IDENT{"newa"}), QuotedIdent{"normSq"}, PartialMatch{"cenX cenX * "});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
        const auto reference_clause2 = std::make_shared<PatternAssignClause>(
            std::make_shared<AssignSynonym>(IDENT{"newa"}), QuotedIdent{"normSq"}, PartialMatch{"cenX "});
        REQUIRE(*(result.clauses[1]) == *reference_clause2);
    }
}