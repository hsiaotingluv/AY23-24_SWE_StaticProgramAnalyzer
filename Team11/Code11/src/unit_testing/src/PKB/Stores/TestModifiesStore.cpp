#include <catch.hpp>

#include "pkb/stores/modifies_store.h"

TEST_CASE("ModifiesStore Tests") {
    ModifiesStore modifies_store;

    SECTION("Adding and verifying that procedure modifies var relationship is added correctly") {
        Procedure p = Procedure("main");
        Variable v = Variable("x");
        modifies_store.add_procedure_modifies_var(p, v);

        REQUIRE(modifies_store.does_procedure_modify_var(p, v));
    }

    SECTION("Adding and verifying that statement modifies var relationship is added correctly") {
        ModifiesStore::StatementNumber s = "1";
        Variable v = Variable("x");
        modifies_store.add_statement_modifies_var(s, v);

        REQUIRE(modifies_store.does_statement_modify_var(s, v));
    }

    SECTION("Retrieving all variables modified by a statement") {
        ModifiesStore::StatementNumber s = "1";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        modifies_store.add_statement_modifies_var(s, v1);
        modifies_store.add_statement_modifies_var(s, v2);

        auto vars = modifies_store.get_vars_modified_by_statement(s);

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Retrieving all variables modified by a procedure") {
        Procedure p = Procedure("main");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        modifies_store.add_procedure_modifies_var(p, v1);
        modifies_store.add_procedure_modifies_var(p, v2);

        auto vars = modifies_store.get_vars_modified_by_procedure(p);

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Retrieving all statements that modify a variable") {
        ModifiesStore::StatementNumber s1 = "1";
        ModifiesStore::StatementNumber s2 = "2";
        Variable v = Variable("x");
        modifies_store.add_statement_modifies_var(s1, v);
        modifies_store.add_statement_modifies_var(s2, v);

        auto stmts = modifies_store.get_statements_that_modify_var(v);

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(s1) == 1);
        REQUIRE(stmts.count(s2) == 1);
    }

    SECTION("Retrieving all procedures that modify a variable") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v = Variable("x");
        modifies_store.add_procedure_modifies_var(p1, v);
        modifies_store.add_procedure_modifies_var(p2, v);

        auto procs = modifies_store.get_procedures_that_modify_var(v);

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(p1) == 1);
        REQUIRE(procs.count(p2) == 1);
    }

    SECTION("Retrieving all statements that modify any variable") {
        ModifiesStore::StatementNumber s1 = "1";
        ModifiesStore::StatementNumber s2 = "2";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        modifies_store.add_statement_modifies_var(s1, v1);
        modifies_store.add_statement_modifies_var(s2, v2);

        auto stmts = modifies_store.get_all_statements_that_modify();

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(s1) == 1);
        REQUIRE(stmts.count(s2) == 1);
    }

    SECTION("Retrieving all procedures that modify any variable") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        modifies_store.add_procedure_modifies_var(p1, v1);
        modifies_store.add_procedure_modifies_var(p2, v2);

        auto procs = modifies_store.get_all_procedures_that_modify();

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(p1) == 1);
        REQUIRE(procs.count(p2) == 1);
    }

    SECTION("Retrieving all statements and var modify pairs") {
        ModifiesStore::StatementNumber s1 = "1";
        ModifiesStore::StatementNumber s2 = "2";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        modifies_store.add_statement_modifies_var(s1, v1);
        modifies_store.add_statement_modifies_var(s2, v2);

        auto pairs = modifies_store.get_all_statements_and_var_pairs();

        REQUIRE(pairs.size() == 2);
        REQUIRE(pairs.count(std::make_tuple(s1, v1)) == 1);
        REQUIRE(pairs.count(std::make_tuple(s2, v2)) == 1);
    }

    SECTION("Retrieving all procedures and var modify pairs") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        modifies_store.add_procedure_modifies_var(p1, v1);
        modifies_store.add_procedure_modifies_var(p2, v2);

        auto pairs = modifies_store.get_all_procedures_and_var_pairs();

        REQUIRE(pairs.size() == 2);
        REQUIRE(pairs.count(std::make_tuple(p1, v1)) == 1);
        REQUIRE(pairs.count(std::make_tuple(p2, v2)) == 1);
    }

    SECTION("Checking if a statement modifies any variable") {
        ModifiesStore::StatementNumber s = "1";
        Variable v = Variable("x");
        modifies_store.add_statement_modifies_var(s, v);

        REQUIRE(modifies_store.does_statement_modify_any_var(s));
    }

    SECTION("Checking if a procedure modifies any variable") {
        Procedure p = Procedure("main");
        Variable v = Variable("x");
        modifies_store.add_procedure_modifies_var(p, v);

        REQUIRE(modifies_store.does_procedure_modify_any_var(p));
    }
}
