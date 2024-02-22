#include <catch.hpp>

#include "pkb/stores/modifies_store/procedure_modifies_store.h"

TEST_CASE("ProcedureModifiesStore Tests") {
    ProcedureModifiesStore procedure_modifies_store;

    SECTION("Adding and verifying that procedure modifies var relationship is added correctly") {
        Procedure p = Procedure("main");
        Variable v = Variable("x");
        procedure_modifies_store.add(p, v);

        REQUIRE(procedure_modifies_store.contains_key_val_pair(p, v));
    }

    SECTION("Retrieving all variables modified by a procedure") {
        Procedure p = Procedure("main");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        procedure_modifies_store.add(p, v1);
        procedure_modifies_store.add(p, v2);

        auto vars = procedure_modifies_store.get_vals_by_key(p);

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Retrieving all procedures that modify a variable") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v = Variable("x");
        procedure_modifies_store.add(p1, v);
        procedure_modifies_store.add(p2, v);

        auto procs = procedure_modifies_store.get_keys_by_val(v);

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(p1) == 1);
        REQUIRE(procs.count(p2) == 1);
    }

    SECTION("Retrieving all procedures that modify any variable") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        procedure_modifies_store.add(p1, v1);
        procedure_modifies_store.add(p2, v2);

        auto procs = procedure_modifies_store.get_all_keys();

        REQUIRE(procs.size() == 2);
        REQUIRE(procs.count(p1) == 1);
        REQUIRE(procs.count(p2) == 1);
    }

    SECTION("Retrieving all variables modified by any procedure") {
        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("helper");
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        procedure_modifies_store.add(p1, v1);
        procedure_modifies_store.add(p2, v2);

        auto vars = procedure_modifies_store.get_all_vals();

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Checking if a procedure modifies any variable") {
        Procedure p = Procedure("main");
        Variable v = Variable("x");
        procedure_modifies_store.add(p, v);

        REQUIRE(procedure_modifies_store.contains_key(p));
    }
}