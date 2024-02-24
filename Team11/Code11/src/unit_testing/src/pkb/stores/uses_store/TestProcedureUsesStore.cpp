#include <catch.hpp>

#include "pkb/stores/uses_store/procedure_uses_store.h"

TEST_CASE("ProcedureUsesStore Tests") {
    ProcedureUsesStore procedure_uses_store;

    SECTION("Adding and verifying that procedure uses var relationship is added correctly") {
        Procedure p = Procedure("main");
        Variable v = Variable("x");
        procedure_uses_store.add(p, v);

        REQUIRE(procedure_uses_store.contains_key_val_pair(p, v));
    }

    SECTION("Retrieving all variables used by a procedure") {
        Procedure p = Procedure("main");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        procedure_uses_store.add(p, v1);
        procedure_uses_store.add(p, v2);

        auto vars = procedure_uses_store.get_vals_by_key(p);

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Retrieving all procedures that use a variable") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v = Variable("x");
        procedure_uses_store.add(p1, v);
        procedure_uses_store.add(p2, v);

        auto procs = procedure_uses_store.get_keys_by_val(v);

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(p1) == 1);
        REQUIRE(procs.count(p2) == 1);
    }

    SECTION("Retrieving all procedures that use any variable") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        procedure_uses_store.add(p1, v1);
        procedure_uses_store.add(p2, v2);

        auto procs = procedure_uses_store.get_all_keys();

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(p1) == 1);
        REQUIRE(procs.count(p2) == 1);
    }

    SECTION("Retrieving all variables used by any procedure") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        procedure_uses_store.add(p1, v1);
        procedure_uses_store.add(p2, v2);

        auto vars = procedure_uses_store.get_all_vals();

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Checking if a procedure uses any variable") {
        Procedure p = Procedure("main");
        Variable v = Variable("x");
        procedure_uses_store.add(p, v);

        REQUIRE(procedure_uses_store.contains_key(p));
    }
}