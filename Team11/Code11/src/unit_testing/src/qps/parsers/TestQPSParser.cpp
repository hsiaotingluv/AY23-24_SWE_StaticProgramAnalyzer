#include "catch.hpp"

#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/attribute.hpp"
#include "qps/parser/entities/attribute_name.hpp"
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

using namespace qps;

TEST_CASE("Test Declaration Parser") {
    const auto runner = tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

    SECTION("Declaration with one synonym") {
        const auto query = "procedure p;";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output =
            untyped::detail::parse_declarations(tokens.begin(), tokens.end(), untyped::DefaultSupportedSynonyms{});

        const auto& [result, rest] = output;

        REQUIRE(result.size() == 1);
        require_value<ProcSynonym>(result[0], "p");
        REQUIRE(std::distance(rest, tokens.end()) == 0);
    }

    SECTION("Declaration with multiple synonyms") {
        const auto query = "procedure p, q, r; Select p";
        const auto tokens = runner.apply_tokeniser(query);
        const auto output =
            untyped::detail::parse_declarations(tokens.begin(), tokens.end(), untyped::DefaultSupportedSynonyms{});

        const auto& [result, rest] = output;

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

        const auto& [result, rest] = output;

        REQUIRE(result.size() == 2);
        require_value<ProcSynonym>(result[0], "p");
        require_value<VarSynonym>(result[1], "v");
        REQUIRE(std::distance(rest, tokens.end()) == 2);
    }

    SECTION("Declaration with invalid keyword") {
        const auto query = "proc p;";
        auto tokens = runner.apply_tokeniser(query);
        const auto output =
            untyped::detail::parse_declarations(tokens.begin(), tokens.end(), untyped::DefaultSupportedSynonyms{});
        const auto& [result, rest] = output;
        REQUIRE(result.empty());
        REQUIRE(std::distance(tokens.cbegin(), rest) == 0);
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
        const auto reference_clause = std::make_shared<PatternClause>(
            PatternAssign{std::make_shared<AssignSynonym>(IDENT{"a"}), WildCard{}, PartialMatch{"count 1 + "}});
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
        const auto reference_clause = std::make_shared<PatternClause>(PatternAssign{
            std::make_shared<AssignSynonym>(IDENT{"newa"}), QuotedIdent{"normSq"}, PartialMatch{"cenX cenX * "}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with pattern while clause") {
        const auto query = R"(while newa;Select newa pattern newa ( "normSq" , _))";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);

        require_value<WhileSynonym>(result.declared[0], "newa");
        require_value<WhileSynonym>(result.reference, "newa");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<PatternClause>(
            PatternWhile{std::make_shared<WhileSynonym>(IDENT{"newa"}), QuotedIdent{"normSq"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with pattern while clause - Invalid Semantics") {
        const auto query = R"(while newa;Select newa pattern newa ( "normSq" , _"cenX"_))";
        const auto output = qps.parse(query);

        REQUIRE(is_semantic_error(output));
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

    SECTION("Query with missing semi-colon") {
        const auto query = "stmt s Select s";
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

        const auto query3 = "Select s";
        const auto output3 = qps.parse(query3);
        REQUIRE(is_semantic_error(output3));
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

TEST_CASE("Test QPS - advanced Relationships") {
    const auto qps = qps::DefaultParser{};

    SECTION("Calls") {
        const auto query = "procedure p; Select p such that Calls(p, \"q\")";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());

        const auto result = output.value();
        REQUIRE(result.declared.size() == 1);
        require_value<ProcSynonym>(result.declared[0], "p");

        require_value<ProcSynonym>(result.reference, "p");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            Calls{ProcedureRef{std::make_shared<ProcSynonym>(IDENT{"p"})}, QuotedIdent{"q"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
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
        const auto reference_clause = std::make_shared<PatternClause>(PatternAssign{
            std::make_shared<AssignSynonym>(IDENT{"newa"}), QuotedIdent{"normSq"}, PartialMatch{"cenX cenX * "}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
        const auto reference_clause2 = std::make_shared<PatternClause>(PatternAssign{
            std::make_shared<AssignSynonym>(IDENT{"newa"}), QuotedIdent{"normSq"}, PartialMatch{"cenX "}});
        REQUIRE(*(result.clauses[1]) == *reference_clause2);
    }
}

TEST_CASE("Test QPS - BOOLEAN") {
    const auto qps = qps::DefaultParser{};

    SECTION("Query with BOOLEAN reference") {
        const auto query = " stmt s; Select BOOLEAN such that Follows*(s, 13)";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);
        require_value<AnyStmtSynonym>(result.declared[0], "s");
        require_boolean(result.reference);

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            FollowsT{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{"13"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with BOOLEAN reference - 2") {
        const auto query = "Select BOOLEAN such that Follows*(_, 13)";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 0);
        require_boolean(result.reference);

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<SuchThatClause>(FollowsT{WildCard{}, Integer{"13"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }

    SECTION("Query with BOOLEAN reference overriden by declaration") {
        const auto query = " stmt s, BOOLEAN; Select BOOLEAN such that Follows*(s, 13) and Modifies(1, \"v\")";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 2);
        require_value<AnyStmtSynonym>(result.declared[0], "s");
        require_value<AnyStmtSynonym>(result.reference, "BOOLEAN");

        REQUIRE(result.clauses.size() == 2);
        const auto reference_clause = std::make_shared<SuchThatClause>(
            FollowsT{StmtRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"})}, Integer{"13"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);

        const auto reference_clause2 = std::make_shared<SuchThatClause>(ModifiesS{Integer{"1"}, QuotedIdent{"v"}});
        REQUIRE(*(result.clauses[1]) == *reference_clause2);
    }

    SECTION("Invalid Semantics - tuple with undeclared BOOLEAN") {
        const auto query = " stmt s; Select <s, BOOLEAN> such that Follows*(s, 13) and Modifies(1, \"v\")";
        const auto output = qps.parse(query);

        REQUIRE(is_semantic_error(output));
    }

    SECTION("Invalid Semantics - tuple with undeclared BOOLEAN - 2") {
        const auto query = " Select <BOOLEAN> such that Follows*(_, 13) and Modifies(1, \"v\")";
        const auto output = qps.parse(query);

        REQUIRE(is_semantic_error(output));
    }
}

TEST_CASE("Test QPS - Pattern If") {
    const auto qps = qps::DefaultParser{};

    SECTION("Query with pattern if clause") {
        const auto query = R"(if ifs;Select ifs pattern ifs ( "normSq" , _, _))";
        const auto output = to_query(qps.parse(query));

        REQUIRE(output.has_value());
        const auto result = output.value();

        REQUIRE(result.declared.size() == 1);
        require_value<IfSynonym>(result.declared[0], "ifs");
        require_value<IfSynonym>(result.reference, "ifs");

        REQUIRE(result.clauses.size() == 1);
        const auto reference_clause = std::make_shared<PatternClause>(
            PatternIf{std::make_shared<IfSynonym>(IDENT{"ifs"}), QuotedIdent{"normSq"}});
        REQUIRE(*(result.clauses[0]) == *reference_clause);
    }
}

TEST_CASE("Test QPS - With") {
    const auto qps = qps::DefaultParser{};

    SECTION("Query - with procName") {
        SECTION("Query - with procedure.procName") {
            const auto query = "procedure p; Select p with p.procName = \"main\"";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);
            require_value<ProcSynonym>(result.declared[0], "p");
            require_value<ProcSynonym>(result.reference, "p");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<ProcSynonym>(IDENT{"p"}), ProcName{}, AttrRef::Type::Name},
                QuotedIdent{"main"});
            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with call.procName") {
            const auto query = "call c; Select c with c.procName = \"main\"";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);
            require_value<CallSynonym>(result.declared[0], "c");
            require_value<CallSynonym>(result.reference, "c");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<CallSynonym>(IDENT{"c"}), ProcName{}, AttrRef::Type::Name},
                QuotedIdent{"main"});
            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with variable.procName - SemanticError") {
            const auto query = "variable v; Select v with v.procName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with stmt.procName - SemanticError") {
            const auto query = "stmt s; Select s with s.procName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with constant.procName - SemanticError") {
            const auto query = "constant c; Select c with c.procName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with print.procName - SemanticError") {
            const auto query = "print p; Select p with p.procName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with read.procName - SemanticError") {
            const auto query = "read r; Select r with r.procName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with while.procName - SemanticError") {
            const auto query = "while w; Select w with w.procName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with if.procName - SemanticError") {
            const auto query = "if i; Select i with i.procName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with assign.procName - SemanticError") {
            const auto query = "assign a; Select a with a.procName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }
    }

    SECTION("Query - with varName") {
        SECTION("Query - with v.varName") {
            const auto query = "variable v; Select v with v.varName = \"main\"";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);
            require_value<VarSynonym>(result.declared[0], "v");
            require_value<VarSynonym>(result.reference, "v");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<VarSynonym>(IDENT{"v"}), VarName{}, AttrRef::Type::Name}, QuotedIdent{"main"});
            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with read.varName") {
            const auto query = "read r; Select r with r.varName = \"main\"";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);
            require_value<ReadSynonym>(result.declared[0], "r");
            require_value<ReadSynonym>(result.reference, "r");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<ReadSynonym>(IDENT{"r"}), VarName{}, AttrRef::Type::Name},
                QuotedIdent{"main"});
            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with print.varName") {
            const auto query = "print p; Select p with p.varName = \"main\"";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);
            require_value<PrintSynonym>(result.declared[0], "p");
            require_value<PrintSynonym>(result.reference, "p");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<PrintSynonym>(IDENT{"p"}), VarName{}, AttrRef::Type::Name},
                QuotedIdent{"main"});
            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with proc.varName - SemanticError") {
            const auto query = "procedure p; Select p with p.varName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with constant.varName - SemanticError") {
            const auto query = "constant c; Select c with c.varName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with stmt.varName - SemanticError") {
            const auto query = "stmt s; Select s with s.varName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with assign.varName - SemanticError") {
            const auto query = "assign a; Select a with a.varName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with while.varName - SemanticError") {
            const auto query = "while w; Select w with w.varName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with if.varName - SemanticError") {
            const auto query = "if i; Select i with i.varName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with call.varName - SemanticError") {
            const auto query = "call c; Select c with c.varName = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }
    }

    SECTION("Query - with value") {
        SECTION("Query - with constant.value") {
            const auto query = "constant c; Select c with c.value = 1";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);
            require_value<ConstSynonym>(result.declared[0], "c");
            require_value<ConstSynonym>(result.reference, "c");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<ConstSynonym>(IDENT{"c"}), Value{}, AttrRef::Type::Integer}, Integer{"1"});
            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with variable.value - SemanticError") {
            const auto query = "variable v; Select v with v.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with read.value - SemanticError") {
            const auto query = "read r; Select r with r.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with print.value - SemanticError") {
            const auto query = "print p; Select p with p.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with proc.value - SemanticError") {
            const auto query = "procedure p; Select p with p.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with stmt.value - SemanticError") {
            const auto query = "stmt s; Select s with s.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with assign.value - SemanticError") {
            const auto query = "assign a; Select a with a.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with while.value - SemanticError") {
            const auto query = "while w; Select w with w.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with if.value - SemanticError") {
            const auto query = "if i; Select i with i.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with call.value - SemanticError") {
            const auto query = "call c; Select c with c.value = \"main\"";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }
    }

    SECTION("Query - with stmt#") {
        SECTION("Query - with s.stmt#") {
            const auto query = "stmt s; Select s with s.stmt# = 1";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);
            require_value<AnyStmtSynonym>(result.declared[0], "s");
            require_value<AnyStmtSynonym>(result.reference, "s");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<AnyStmtSynonym>(IDENT{"s"}), StmtNum{}, AttrRef::Type::Integer}, Integer{"1"});

            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with r.stmt#") {
            const auto query = "read r; Select r with r.stmt# = 1";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);

            require_value<ReadSynonym>(result.declared[0], "r");
            require_value<ReadSynonym>(result.reference, "r");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<ReadSynonym>(IDENT{"r"}), StmtNum{}, AttrRef::Type::Integer}, Integer{"1"});

            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with print.stmt#") {
            const auto query = "print p; Select p with p.stmt# = 1";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);

            require_value<PrintSynonym>(result.declared[0], "p");
            require_value<PrintSynonym>(result.reference, "p");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<PrintSynonym>(IDENT{"p"}), StmtNum{}, AttrRef::Type::Integer}, Integer{"1"});

            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with call.stmt#") {
            const auto query = "call c; Select c with c.stmt# = 1";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);

            require_value<CallSynonym>(result.declared[0], "c");
            require_value<CallSynonym>(result.reference, "c");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<CallSynonym>(IDENT{"c"}), StmtNum{}, AttrRef::Type::Integer}, Integer{"1"});

            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with while.stmt#") {
            const auto query = "while w; Select w with w.stmt# = 1";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);

            require_value<WhileSynonym>(result.declared[0], "w");
            require_value<WhileSynonym>(result.reference, "w");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<WhileSynonym>(IDENT{"w"}), StmtNum{}, AttrRef::Type::Integer}, Integer{"1"});

            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with if.stmt#") {
            const auto query = "if i; Select i with i.stmt# = 1";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);

            require_value<IfSynonym>(result.declared[0], "i");
            require_value<IfSynonym>(result.reference, "i");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<IfSynonym>(IDENT{"i"}), StmtNum{}, AttrRef::Type::Integer}, Integer{"1"});

            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with assign.stmt#") {
            const auto query = "assign a; Select a with a.stmt# = 1";
            const auto output = to_query(qps.parse(query));

            REQUIRE(output.has_value());
            const auto result = output.value();

            REQUIRE(result.declared.size() == 1);

            require_value<AssignSynonym>(result.declared[0], "a");
            require_value<AssignSynonym>(result.reference, "a");

            REQUIRE(result.clauses.size() == 1);
            const auto reference_clause = std::make_shared<WithClause>(
                AttrRef{std::make_shared<AssignSynonym>(IDENT{"a"}), StmtNum{}, AttrRef::Type::Integer}, Integer{"1"});

            REQUIRE(*(result.clauses[0]) == *reference_clause);
        }

        SECTION("Query - with p.stmt# - SemanticError") {
            const auto query = "procedure p; Select p with p.stmt# = 1";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with c.stmt# - SemanticError") {
            const auto query = "constant c; Select c with c.stmt# = 1";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }

        SECTION("Query - with v.stmt# - SemanticError") {
            const auto query = "variable v; Select v with v.stmt# = 1";
            const auto output = qps.parse(query);

            REQUIRE(is_semantic_error(output));
        }
    }
}

TEST_CASE("Test QPS - Select attrRef") {
    const auto qps = qps::DefaultParser{};

    SECTION("Query - Select attrRef one") {
        const auto query = "stmt s; Select s.stmt#";

        const auto output = to_query(qps.parse(query));
        REQUIRE(output.has_value());

        const auto result = output.value();
        REQUIRE(result.declared.size() == 1);
        require_value<AnyStmtSynonym>(result.declared[0], "s");
        require_value<AnyStmtSynonym>(result.reference, "s");
    }

    SECTION("Query - Select attrRef two") {
        const auto query = "assign a1; variable a2; Select<a1.stmt#, a2> such that Modifies(a1, a2)";

        const auto output = to_query(qps.parse(query));
        REQUIRE(output.has_value());

        const auto result = output.value();
        REQUIRE(result.declared.size() == 2);
        require_value<AssignSynonym>(result.declared[0], "a1");
        require_value<VarSynonym>(result.declared[1], "a2");

        require_value<AssignSynonym>(result.reference, "a1");
        require_value<VarSynonym>(result.reference, "a2");
    }

    SECTION("Query - Invalid Syntax") {
        const auto query = "assign a1; Select<a1.stmt #>";
        const auto output = qps.parse(query);
        REQUIRE(is_syntax_error(output));
    }
}