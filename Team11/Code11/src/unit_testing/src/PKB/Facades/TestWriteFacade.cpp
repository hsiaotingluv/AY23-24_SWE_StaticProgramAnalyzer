#include "catch.hpp"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb.h"
#include <memory>

// Mock PKB for testing
class MockPKB : public PKB {};

std::shared_ptr<PKB> createMockPKB() {
    return std::make_shared<MockPKB>();
}

TEST_CASE("WriteFacade add_procedure Test") {
    auto mockPKB = createMockPKB();
    WriteFacade writeFacade(mockPKB);

    SECTION("add_procedure") {
        REQUIRE_NOTHROW(writeFacade.add_procedure("TestProcedure"));
    }

    SECTION("add_variable") {
        REQUIRE_NOTHROW(writeFacade.add_variable("TestVariable"));
    }

    SECTION("add_constant") {
        REQUIRE_NOTHROW(writeFacade.add_constant("123"));
    }

    SECTION("add_statement") {
        REQUIRE_NOTHROW(writeFacade.add_statement("1", StatementType::Assign));
    }

    SECTION("add_statement_modifies_var") {
        REQUIRE_NOTHROW(writeFacade.add_statement_modifies_var("1", "TestVariable"));
    }

    SECTION("add_procedure_modifies_var") {
        REQUIRE_NOTHROW(writeFacade.add_procedure_modifies_var("TestProcedure", "TestVariable"));
    }

    SECTION("add_statement_uses_var") {
        REQUIRE_NOTHROW(writeFacade.add_statement_uses_var("1", "TestVariable"));
    }

    SECTION("add_procedure_uses_var") {
        REQUIRE_NOTHROW(writeFacade.add_procedure_uses_var("TestProcedure", "TestVariable"));
    }

    SECTION("add_follows") {
        REQUIRE_NOTHROW(writeFacade.add_follows("1", "2"));
    }

    SECTION("add_parent") {
        REQUIRE_NOTHROW(writeFacade.add_parent("1", "4"));
    }

    SECTION("add_next") {
        REQUIRE_NOTHROW(writeFacade.add_next("1", "6"));
    }

    SECTION("add_next_star") {
        REQUIRE_NOTHROW(writeFacade.add_next_star("1", "7"));
    }

    SECTION("add_calls") {
        REQUIRE_NOTHROW(writeFacade.add_calls("CallerProcedure", "CalleeProcedure"));
    }

    SECTION("add_calls_star") {
        REQUIRE_NOTHROW(writeFacade.add_calls_star("CallerProcedure", "CalleeProcedureStar"));
    }
}
