#include <catch.hpp>

#include "PKB/Stores/CallsStore.h"

TEST_CASE("CallsStore Tests") {
    CallsStore callsStore;

    SECTION("Adding and Verifying Direct Calls Relationships") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCalls("Helper", "Logger");

        REQUIRE(callsStore.hasCalls("Main", "Helper"));
        REQUIRE(callsStore.hasCalls("Helper", "Logger"));
    }

    SECTION("Verifying Absence of Non-existent Direct Calls Relationships") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCalls("Helper", "Logger");

        REQUIRE_FALSE(callsStore.hasCalls("Main", "Logger"));
        REQUIRE_FALSE(callsStore.hasCalls("Helper", "Main"));
        REQUIRE_FALSE(callsStore.hasCalls("Logger", "Helper"));
    }

    SECTION("Get All Direct Calls Relationships") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCalls("Main", "Logger");
        callsStore.addCalls("Helper", "Validator");

        auto allCalls = callsStore.getAllCalls();

        REQUIRE(allCalls.size() == 2);
        REQUIRE(allCalls["Main"].size() == 2);
        REQUIRE(allCalls["Main"].count("Helper") == 1);
        REQUIRE(allCalls["Main"].count("Logger") == 1);
        REQUIRE(allCalls["Helper"].size() == 1);
        REQUIRE(allCalls["Helper"].count("Validator") == 1);

        REQUIRE(allCalls.count("Logger") == 0);
        REQUIRE(allCalls.count("Validator") == 0);
    }

    SECTION("Retrieving All Callees") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCalls("Main", "Utils");

        auto callees = callsStore.getAllCallsValues();

        REQUIRE(callees.size() == 2);
        REQUIRE(callees.find("Main") == callees.end());
        REQUIRE(callees.find("Helper") != callees.end());
        REQUIRE(callees.find("Utils") != callees.end());
    }

    SECTION("Retrieving All Callers") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCalls("Utils", "Logger");

        auto callers = callsStore.getAllCallsKeys();

        REQUIRE(callers.size() == 2);
        REQUIRE(callers.find("Main") != callers.end());
        REQUIRE(callers.find("Utils") != callers.end());
        REQUIRE(callers.find("Helper") == callers.end());
        REQUIRE(callers.find("Logger") == callers.end());
    }

    SECTION("Retrieving All Callees for a Given Caller") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCalls("Main", "Logger");

        auto callees = callsStore.getCallees("Main");

        REQUIRE(callees.size() == 2);
        REQUIRE(callees.find("Helper") != callees.end());
        REQUIRE(callees.find("Logger") != callees.end());
        REQUIRE(callees.find("Main") == callees.end());
    }

    SECTION("Retrieving Callees for Non-existent Caller Returns Empty Set") {
        auto callees = callsStore.getCallees("NonExisting");

        REQUIRE(callees.empty());
    }

    SECTION("Retrieving All Callers for a Given Callee") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCalls("Logger", "Helper");

        auto callers = callsStore.getCallers("Helper");

        REQUIRE(callers.size() == 2);
        REQUIRE(callers.find("Main") != callers.end());
        REQUIRE(callers.find("Logger") != callers.end());
        REQUIRE(callers.find("Helper") == callers.end());
    }

    SECTION("Retrieving Callers for Non-existent Callee Returns Empty Set") {
        auto callers = callsStore.getCallers("NonExisting");

        REQUIRE(callers.empty());
    }

    SECTION("Adding and Verifying Calls* Relationships") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCallsStar("Main", "Utils");

        REQUIRE(callsStore.hasCallsStar("Main", "Helper"));
        REQUIRE(callsStore.hasCallsStar("Main", "Utils"));
    }

    SECTION("Verifying Absence of Non-existent Calls* Relationships") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCallsStar("Main", "Utils");

        REQUIRE_FALSE(callsStore.hasCallsStar("Utils", "Main"));
        REQUIRE_FALSE(callsStore.hasCallsStar("Helper", "Main"));
        REQUIRE_FALSE(callsStore.hasCallsStar("Helper", "Utils"));
    }

    SECTION("Getting All Transitive Calls Relationships") {
        callsStore.addCalls("Main", "Helper");
        callsStore.addCalls("Helper", "Logger");
        callsStore.addCallsStar("Main", "Logger");

        auto allCallsStar = callsStore.getAllCallsStar();

        REQUIRE(allCallsStar.size() == 2);
        REQUIRE(allCallsStar["Main"].size() == 2);
        REQUIRE(allCallsStar["Main"].count("Helper") == 1);
        REQUIRE(allCallsStar["Main"].count("Logger") == 1);
        REQUIRE(allCallsStar["Helper"].count("Logger") == 1);

        REQUIRE(allCallsStar.count("Logger") == 0);
    }

    SECTION("Retrieving All Transitive Callees") {
        callsStore.addCallsStar("Main", "Utils");
        callsStore.addCallsStar("Main", "Logger");

        auto starCallees = callsStore.getAllCallsStarValues();

        REQUIRE(starCallees.size() == 2);
        REQUIRE(starCallees.find("Utils") != starCallees.end());
        REQUIRE(starCallees.find("Logger") != starCallees.end());
        REQUIRE(starCallees.find("Main") == starCallees.end());
    }

    SECTION("Retrieving All Transitive Callers") {
        callsStore.addCallsStar("Main", "Utils");
        callsStore.addCallsStar("Logger", "Utils");

        auto starCallers = callsStore.getAllCallsStarKeys();

        REQUIRE(starCallers.size() == 2);
        REQUIRE(starCallers.find("Main") != starCallers.end());
        REQUIRE(starCallers.find("Logger") != starCallers.end());
        REQUIRE(starCallers.find("Utils") == starCallers.end());
    }

    SECTION("Retrieving All Transitive Callees for a Given Caller") {
        callsStore.addCallsStar("Main", "Utils");

        auto starCallees = callsStore.getCallsStarCallees("Main");

        REQUIRE(starCallees.find("Utils") != starCallees.end());
        REQUIRE_FALSE(starCallees.find("Main") != starCallees.end());
    }

    SECTION("Retrieving Transitive Callees for Non-existent Caller Returns Empty Set") {
        auto starCallees = callsStore.getCallsStarCallees("NonExisting");

        REQUIRE(starCallees.empty());
    }

    SECTION("Retrieving All Transitive Callers for a Given Callee") {
        callsStore.addCallsStar("Main", "Utils");
        callsStore.addCallsStar("Helper", "Utils");

        auto starCallers = callsStore.getCallsStarCallers("Utils");

        REQUIRE(starCallers.size() == 2);
        REQUIRE(starCallers.find("Main") != starCallers.end());
        REQUIRE(starCallers.find("Helper") != starCallers.end());
        REQUIRE(starCallers.find("Utils") == starCallers.end());
    }

    SECTION("Retrieving Transitive Callers for Non-existent Callee Returns Empty Set") {
        auto starCallers = callsStore.getCallsStarCallers("NonExisting");

        REQUIRE(starCallers.empty());
    }
}
