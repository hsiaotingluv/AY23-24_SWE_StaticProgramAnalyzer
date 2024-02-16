#include <catch.hpp>

#include "pkb/stores/uses_store.h"

TEST_CASE("UsesStore Tests") {
    UsesStore uses_store;

    SECTION("Adding and verifying that procedure uses var relationship is added correctly") {
        Procedure p = Procedure("main");
        Variable v = Variable("x");
        uses_store.add_procedure_uses_var(p, v);

        REQUIRE(uses_store.does_procedure_use_var(p, v));
    }

    SECTION("Adding and verifying that statement uses var relationship is added correctly") {
        UsesStore::StatementNumber s = "1";
        Variable v = Variable("x");
        uses_store.add_statement_uses_var(s, v);

        REQUIRE(uses_store.does_statement_use_var(s, v));
    }

    SECTION("Retrieving all variables used by a statement") {
        UsesStore::StatementNumber s = "1";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        uses_store.add_statement_uses_var(s, v1);
        uses_store.add_statement_uses_var(s, v2);

        auto vars = uses_store.get_vars_used_by_statement(s);

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Retrieving all variables used by a procedure") {
        Procedure p = Procedure("main");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        uses_store.add_procedure_uses_var(p, v1);
        uses_store.add_procedure_uses_var(p, v2);

        auto vars = uses_store.get_vars_used_by_procedure(p);

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Retrieving all statements that use a variable") {
        UsesStore::StatementNumber s1 = "1";
        UsesStore::StatementNumber s2 = "2";
        Variable v = Variable("x");
        uses_store.add_statement_uses_var(s1, v);
        uses_store.add_statement_uses_var(s2, v);

        auto stmts = uses_store.get_statements_that_use_var(v);

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(s1) == 1);
        REQUIRE(stmts.count(s2) == 1);
    }

    SECTION("Retrieving all procedures that use a variable") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v = Variable("x");
        uses_store.add_procedure_uses_var(p1, v);
        uses_store.add_procedure_uses_var(p2, v);

        auto procs = uses_store.get_procedures_that_use_var(v);

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(p1) == 1);
        REQUIRE(procs.count(p2) == 1);
    }

    SECTION("Retrieving all statements that use any variable") {
        UsesStore::StatementNumber s1 = "1";
        UsesStore::StatementNumber s2 = "2";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        uses_store.add_statement_uses_var(s1, v1);
        uses_store.add_statement_uses_var(s2, v2);

        auto stmts = uses_store.get_all_statements_that_use();

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(s1) == 1);
        REQUIRE(stmts.count(s2) == 1);
    }

    SECTION("Checking if a statement uses any variable") {
        UsesStore::StatementNumber s = "1";
        Variable v = Variable("x");
        uses_store.add_statement_uses_var(s, v);

        REQUIRE(uses_store.does_statement_use_any_var(s));
    }

    SECTION("Retrieving all procedures that use any variable") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        uses_store.add_procedure_uses_var(p1, v1);
        uses_store.add_procedure_uses_var(p2, v2);

        auto procs = uses_store.get_all_procedures_that_use();

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(p1) == 1);
        REQUIRE(procs.count(p2) == 1);
    }

    SECTION("Checking if a procedure uses any variable") {
        Procedure p = Procedure("main");
        Variable v = Variable("x");
        uses_store.add_procedure_uses_var(p, v);

        REQUIRE(uses_store.does_procedure_use_any_var(p));
    }

    SECTION("Retrieving all statements and var pairs") {
        UsesStore::StatementNumber s1 = "1";
        UsesStore::StatementNumber s2 = "2";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        uses_store.add_statement_uses_var(s1, v1);
        uses_store.add_statement_uses_var(s2, v2);

        auto stmts = uses_store.get_all_statements_and_var_pairs();

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(std::make_tuple(s1, v1)) == 1);
        REQUIRE(stmts.count(std::make_tuple(s2, v2)) == 1);
    }

    SECTION("Retrieving all procedures and var pairs") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        uses_store.add_procedure_uses_var(p1, v1);
        uses_store.add_procedure_uses_var(p2, v2);

        auto procs = uses_store.get_all_procedures_and_var_pairs();

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(std::make_tuple(p1, v1)) == 1);
        REQUIRE(procs.count(std::make_tuple(p2, v2)) == 1);
    }
}
