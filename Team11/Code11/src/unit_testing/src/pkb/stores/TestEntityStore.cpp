#include "pkb/stores/entity_store.h"
#include <catch.hpp>

TEST_CASE("EntityStore Tests") {
    EntityStore entity_store;

    SECTION("Adding Variables") {
        entity_store.add_variable(Variable("x"));
        entity_store.add_variable(Variable("y"));

        auto variables = entity_store.get_variables();

        REQUIRE(variables.size() == 2);
        REQUIRE(variables.find(Variable("x")) != variables.end());
        REQUIRE(variables.find(Variable("y")) != variables.end());
        REQUIRE(variables.find(Variable("z")) == variables.end());
    }

    SECTION("Adding Procedures") {
        entity_store.add_procedure(Procedure("main"));
        entity_store.add_procedure(Procedure("helper"));

        auto procedures = entity_store.get_procedures();

        REQUIRE(procedures.size() == 2);
        REQUIRE(procedures.find(Procedure("main")) != procedures.end());
        REQUIRE(procedures.find(Procedure("helper")) != procedures.end());
        REQUIRE(procedures.find(Procedure("nonexistent")) == procedures.end());
    }

    SECTION("Adding Constants") {
        entity_store.add_constant(Constant("1"));
        entity_store.add_constant(Constant("2"));

        auto constants = entity_store.get_constants();

        REQUIRE(constants.size() == 2);
        REQUIRE(constants.find(Constant("1")) != constants.end());
        REQUIRE(constants.find(Constant("2")) != constants.end());
        REQUIRE(constants.find(Constant("3")) == constants.end());
    }

    SECTION("Entity uniqueness") {
        entity_store.add_variable(Variable("x"));
        entity_store.add_variable(Variable("x"));
        entity_store.add_procedure(Procedure("main"));
        entity_store.add_procedure(Procedure("main"));
        entity_store.add_constant(Constant("1"));
        entity_store.add_constant(Constant("1"));

        auto variables = entity_store.get_variables();
        auto procedures = entity_store.get_procedures();
        auto constants = entity_store.get_constants();

        REQUIRE(variables.size() == 1);
        REQUIRE(variables.find(Variable("x")) != variables.end());

        REQUIRE(procedures.size() == 1);
        REQUIRE(procedures.find(Procedure("main")) != procedures.end());

        REQUIRE(constants.size() == 1);
        REQUIRE(constants.find(Constant("1")) != constants.end());
    }

    SECTION("Initial Store state") {
        REQUIRE(entity_store.get_variables().empty());
        REQUIRE(entity_store.get_procedures().empty());
        REQUIRE(entity_store.get_constants().empty());
    }

    SECTION("Adding Entities after initial check") {
        REQUIRE(entity_store.get_variables().empty());
        REQUIRE(entity_store.get_procedures().empty());
        REQUIRE(entity_store.get_constants().empty());

        entity_store.add_variable(Variable("v"));
        entity_store.add_procedure(Procedure("p"));
        entity_store.add_constant(Constant("c"));

        REQUIRE(entity_store.get_variables().size() == 1);
        REQUIRE(entity_store.get_procedures().size() == 1);
        REQUIRE(entity_store.get_constants().size() == 1);
    }

    SECTION("Mixed Entities handling") {
        entity_store.add_variable(Variable("v"));
        entity_store.add_procedure(Procedure("p"));
        entity_store.add_constant(Constant("c"));

        auto variables = entity_store.get_variables();
        auto procedures = entity_store.get_procedures();
        auto constants = entity_store.get_constants();

        REQUIRE(variables.find(Variable("v")) != variables.end());
        REQUIRE(procedures.find(Procedure("p")) != procedures.end());
        REQUIRE(constants.find(Constant("c")) != constants.end());

        REQUIRE(variables.find(Variable("p")) == variables.end());
        REQUIRE(procedures.find(Procedure("c")) == procedures.end());
        REQUIRE(constants.find(Constant("v")) == constants.end());
    }
}
