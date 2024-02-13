#include "PKB/Stores/EntityStore.h"
#include <catch.hpp>

TEST_CASE("EntityStore Tests") {
    EntityStore entityStore;

    SECTION("Adding Variables") {
        entityStore.addVariable(Variable("x"));
        entityStore.addVariable(Variable("y"));

        auto variables = entityStore.getVariables();

        REQUIRE(variables.size() == 2);
        REQUIRE(variables.find(Variable("x")) != variables.end());
        REQUIRE(variables.find(Variable("y")) != variables.end());
        REQUIRE(variables.find(Variable("z")) == variables.end());
    }

    SECTION("Adding Procedures") {
        entityStore.addProcedure(Procedure("main"));
        entityStore.addProcedure(Procedure("helper"));

        auto procedures = entityStore.getProcedures();

        REQUIRE(procedures.size() == 2);
        REQUIRE(procedures.find(Procedure("main")) != procedures.end());
        REQUIRE(procedures.find(Procedure("helper")) != procedures.end());
        REQUIRE(procedures.find(Procedure("nonexistent")) == procedures.end());
    }

    SECTION("Adding Constants") {
        entityStore.addConstant(Constant("1"));
        entityStore.addConstant(Constant("2"));

        auto constants = entityStore.getConstants();

        REQUIRE(constants.size() == 2);
        REQUIRE(constants.find(Constant("1")) != constants.end());
        REQUIRE(constants.find(Constant("2")) != constants.end());
        REQUIRE(constants.find(Constant("3")) == constants.end());
    }

    SECTION("Entity uniqueness") {
        entityStore.addVariable(Variable("x"));
        entityStore.addVariable(Variable("x"));
        entityStore.addProcedure(Procedure("main"));
        entityStore.addProcedure(Procedure("main"));
        entityStore.addConstant(Constant("1"));
        entityStore.addConstant(Constant("1"));

        auto variables = entityStore.getVariables();
        auto procedures = entityStore.getProcedures();
        auto constants = entityStore.getConstants();

        REQUIRE(variables.size() == 1);
        REQUIRE(variables.find(Variable("x")) != variables.end());

        REQUIRE(procedures.size() == 1);
        REQUIRE(procedures.find(Procedure("main")) != procedures.end());

        REQUIRE(constants.size() == 1);
        REQUIRE(constants.find(Constant("1")) != constants.end());
    }

    SECTION("Initial Store state") {
        REQUIRE(entityStore.getVariables().empty());
        REQUIRE(entityStore.getProcedures().empty());
        REQUIRE(entityStore.getConstants().empty());
    }

    SECTION("Adding Entities after initial check") {
        REQUIRE(entityStore.getVariables().empty());
        REQUIRE(entityStore.getProcedures().empty());
        REQUIRE(entityStore.getConstants().empty());

        entityStore.addVariable(Variable("v"));
        entityStore.addProcedure(Procedure("p"));
        entityStore.addConstant(Constant("c"));

        REQUIRE(entityStore.getVariables().size() == 1);
        REQUIRE(entityStore.getProcedures().size() == 1);
        REQUIRE(entityStore.getConstants().size() == 1);
    }

    SECTION("Mixed Entities handling") {
        entityStore.addVariable(Variable("v"));
        entityStore.addProcedure(Procedure("p"));
        entityStore.addConstant(Constant("c"));

        REQUIRE(entityStore.getVariables().find(Variable("v")) != entityStore.getVariables().end());
        REQUIRE(entityStore.getProcedures().find(Procedure("p")) != entityStore.getProcedures().end());
        REQUIRE(entityStore.getConstants().find(Constant("c")) != entityStore.getConstants().end());

        REQUIRE(entityStore.getVariables().find(Variable("p")) == entityStore.getVariables().end());
        REQUIRE(entityStore.getProcedures().find(Procedure("c")) == entityStore.getProcedures().end());
        REQUIRE(entityStore.getConstants().find(Constant("v")) == entityStore.getConstants().end());
    }
}
