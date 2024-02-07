#include <catch.hpp>

#include "pkb/Facades/ReadFacade.h"
#include "pkb/Facades/WriteFacade.h"

TEST_CASE("Simple Entity Test") {
    SECTION("Procedure Test") {
        PKB pkb = PKB();

        auto readFacade = std::make_unique<ReadFacade>(pkb);
        auto writeFacade = std::make_unique<WriteFacade>(pkb);

        ReadFacade::Procedure procedure1 = "procedure1";

        writeFacade->addProcedure(procedure1);

        REQUIRE(readFacade->getProcedures().size() == 1);
        REQUIRE(readFacade->getVariables().empty());
        REQUIRE(readFacade->getEntities().size() == 1);
    }
}