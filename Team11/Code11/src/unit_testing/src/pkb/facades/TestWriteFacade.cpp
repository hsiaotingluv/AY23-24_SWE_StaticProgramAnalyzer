#include "catch.hpp"

#include "pkb/facades/write_facade.h"

using namespace pkb;
TEST_CASE("write_facade Test") {
    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();

    SECTION("add_procedure") {
        REQUIRE_NOTHROW(write_facade->add_procedure("TestProcedure"));
    }

    SECTION("add_variable") {
        REQUIRE_NOTHROW(write_facade->add_variable("TestVariable"));
    }

    SECTION("add_constant") {
        REQUIRE_NOTHROW(write_facade->add_constant("123"));
    }

    SECTION("add_statement") {
        REQUIRE_NOTHROW(write_facade->add_statement("1", StatementType::Assign));
    }

    SECTION("add_statement_modifies_var") {
        REQUIRE_NOTHROW(write_facade->add_statement_modifies_var("1", "TestVariable"));
    }

    SECTION("add_procedure_modifies_var") {
        REQUIRE_NOTHROW(write_facade->add_procedure_modifies_var("TestProcedure", "TestVariable"));
    }

    SECTION("add_statement_uses_var") {
        REQUIRE_NOTHROW(write_facade->add_statement_uses_var("1", "TestVariable"));
    }

    SECTION("add_procedure_uses_var") {
        REQUIRE_NOTHROW(write_facade->add_procedure_uses_var("TestProcedure", "TestVariable"));
    }

    SECTION("add_follows") {
        REQUIRE_NOTHROW(write_facade->add_follows("1", "2"));
    }

    SECTION("add_parent") {
        REQUIRE_NOTHROW(write_facade->add_parent("1", "4"));
    }

    SECTION("add_assignment") {
        REQUIRE_NOTHROW(write_facade->add_assignment("1", "x", "2"));
    }

    SECTION("add_next") {
        REQUIRE_NOTHROW(write_facade->add_next("1", "2"));
    }

    SECTION("add_calls") {
        REQUIRE_NOTHROW(write_facade->add_calls("p1", "p2"));
    }

    SECTION("add_if_var") {
        REQUIRE_NOTHROW(write_facade->add_if_var("1", "x"));
    }

    SECTION("add_while_var") {
        REQUIRE_NOTHROW(write_facade->add_while_var("2", "y"));
    }
}
