#include <catch.hpp>
#include <memory>

#include "PKB/Facades/ReadFacade.h"
#include "PKB/Facades/WriteFacade.h"

TEST_CASE("Simple Entity Test") {
    SECTION("Procedure Test") {
        std::shared_ptr<PKB> pkb = std::make_shared<PKB>();

        auto readFacade = std::make_shared<ReadFacade>(pkb);
        auto writeFacade = std::make_shared<WriteFacade>(pkb);

        writeFacade->addProcedure("procedure1");

        REQUIRE(readFacade->getProcedures().size() == 1);
        REQUIRE(readFacade->getVariables().empty());
        REQUIRE(readFacade->getEntities().size() == 1);
    }
}

TEST_CASE("Simple Statement Test") {
    SECTION("Statement Test") {
        std::shared_ptr<PKB> pkb = std::make_shared<PKB>();

        auto readFacade = std::make_shared<ReadFacade>(pkb);
        auto writeFacade = std::make_shared<WriteFacade>(pkb);

        writeFacade->addStatement("1", StatementType::READ);
        writeFacade->addStatement("2", StatementType::PRINT);
        writeFacade->addStatement("3", StatementType::ASSIGN);
        writeFacade->addStatement("4", StatementType::IF);
        writeFacade->addStatement("5", StatementType::WHILE);
        writeFacade->addStatement("6", StatementType::CALL);
        writeFacade->addStatement("7", StatementType::RAW);

        REQUIRE(readFacade->getAllStatements().size() == 7);
        REQUIRE(readFacade->getReadStatements().size() == 1);
        REQUIRE(readFacade->getAssignStatements().size() == 1);
        REQUIRE(readFacade->getPrintStatements().size() == 1);
        REQUIRE(readFacade->getIfStatements().size() == 1);
        REQUIRE(readFacade->getWhileStatements().size() == 1);
        REQUIRE(readFacade->getCallStatements().size() == 1);
        REQUIRE(readFacade->getRawStatements().size() == 1);
    }
}

TEST_CASE("Simple Modify Test") {
    SECTION("Modify Test") {
        std::shared_ptr<PKB> pkb = std::make_shared<PKB>();

        auto readFacade = std::make_shared<ReadFacade>(pkb);
        auto writeFacade = std::make_shared<WriteFacade>(pkb);

        writeFacade->addStatementModifiesVar("1", "x");
        writeFacade->addStatementModifiesVar("1", "y");
        writeFacade->addStatementModifiesVar("2", "x");
        writeFacade->addStatementModifiesVar("2", "z");

        REQUIRE(readFacade->doesStatementModifyVar("1", "x"));
        REQUIRE(!readFacade->doesStatementModifyVar("1", "z"));
        REQUIRE(readFacade->getStatementsThatModifyVar("x").size() == 2);
        REQUIRE(readFacade->getStatementsThatModifyVar("z").size() == 1);
        REQUIRE(readFacade->getVarsModifiedByStatement("1").size() == 2);
    }
}

TEST_CASE("Simple Use Test") {
    SECTION("Use Test") {
        std::shared_ptr<PKB> pkb = std::make_shared<PKB>();

        auto readFacade = std::make_shared<ReadFacade>(pkb);
        auto writeFacade = std::make_shared<WriteFacade>(pkb);

        writeFacade->addStatementUsesVar("1", "x");
        writeFacade->addStatementUsesVar("1", "y");
        writeFacade->addStatementUsesVar("2", "x");
        writeFacade->addStatementUsesVar("2", "z");

        REQUIRE(readFacade->doesStatementUseVar("1", "x"));
        REQUIRE(!readFacade->doesStatementUseVar("1", "z"));
        REQUIRE(readFacade->getStatementsThatUseVar("x").size() == 2);
        REQUIRE(readFacade->getStatementsThatUseVar("z").size() == 1);
        REQUIRE(readFacade->getVarsUsedByStatement("1").size() == 2);
    }
}