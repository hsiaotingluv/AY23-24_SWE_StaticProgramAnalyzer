#include "pkb/stores/entity_store.h"
#include <catch.hpp>

TEST_CASE("EntityStore Tests") {
    EntityStore entityStore;

    SECTION("Adding Variables") {
        entityStore.add_variable(Variable("x"));
        entityStore.add_variable(Variable("y"));

        auto variables = entityStore.get_variables();

        REQUIRE(variables.size() == 2);
        REQUIRE(variables.find(Variable("x")) != variables.end());
        REQUIRE(variables.find(Variable("y")) != variables.end());
        REQUIRE(variables.find(Variable("z")) == variables.end());
    }

    SECTION("Adding Procedures") {
        entityStore.add_procedure(Procedure("main"));
        entityStore.add_procedure(Procedure("helper"));

        auto procedures = entityStore.get_procedures();

        REQUIRE(procedures.size() == 2);
        REQUIRE(procedures.find(Procedure("main")) != procedures.end());
        REQUIRE(procedures.find(Procedure("helper")) != procedures.end());
        REQUIRE(procedures.find(Procedure("nonexistent")) == procedures.end());
    }

    SECTION("Adding Constants") {
        entityStore.add_constant(Constant("1"));
        entityStore.add_constant(Constant("2"));

        auto constants = entityStore.get_constants();

        REQUIRE(constants.size() == 2);
        REQUIRE(constants.find(Constant("1")) != constants.end());
        REQUIRE(constants.find(Constant("2")) != constants.end());
        REQUIRE(constants.find(Constant("3")) == constants.end());
    }

    SECTION("Entity uniqueness") {
        entityStore.add_variable(Variable("x"));
        entityStore.add_variable(Variable("x"));
        entityStore.add_procedure(Procedure("main"));
        entityStore.add_procedure(Procedure("main"));
        entityStore.add_constant(Constant("1"));
        entityStore.add_constant(Constant("1"));

        auto variables = entityStore.get_variables();
        auto procedures = entityStore.get_procedures();
        auto constants = entityStore.get_constants();

        REQUIRE(variables.size() == 1);
        REQUIRE(variables.find(Variable("x")) != variables.end());

        REQUIRE(procedures.size() == 1);
        REQUIRE(procedures.find(Procedure("main")) != procedures.end());

        REQUIRE(constants.size() == 1);
        REQUIRE(constants.find(Constant("1")) != constants.end());
    }

    SECTION("Initial Store state") {
        REQUIRE(entityStore.get_variables().empty());
        REQUIRE(entityStore.get_procedures().empty());
        REQUIRE(entityStore.get_constants().empty());
    }

    SECTION("Adding Entities after initial check") {
        REQUIRE(entityStore.get_variables().empty());
        REQUIRE(entityStore.get_procedures().empty());
        REQUIRE(entityStore.get_constants().empty());

        entityStore.add_variable(Variable("v"));
        entityStore.add_procedure(Procedure("p"));
        entityStore.add_constant(Constant("c"));

        REQUIRE(entityStore.get_variables().size() == 1);
        REQUIRE(entityStore.get_procedures().size() == 1);
        REQUIRE(entityStore.get_constants().size() == 1);
    }

    SECTION("Mixed Entities handling") {
        entityStore.add_variable(Variable("v"));
        entityStore.add_procedure(Procedure("p"));
        entityStore.add_constant(Constant("c"));

        REQUIRE(entityStore.get_variables().find(Variable("v")) != entityStore.get_variables().end());
        REQUIRE(entityStore.get_procedures().find(Procedure("p")) != entityStore.get_procedures().end());
        REQUIRE(entityStore.get_constants().find(Constant("c")) != entityStore.get_constants().end());

        REQUIRE(entityStore.get_variables().find(Variable("p")) == entityStore.get_variables().end());
        REQUIRE(entityStore.get_procedures().find(Procedure("c")) == entityStore.get_procedures().end());
        REQUIRE(entityStore.get_constants().find(Constant("v")) == entityStore.get_constants().end());
    }
}
