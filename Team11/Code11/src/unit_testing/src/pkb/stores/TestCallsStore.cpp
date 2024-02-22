#include <catch.hpp>

#include "pkb/stores/calls_store.h"

TEST_CASE("CallsStore Tests") {
    CallsStore calls_store;

    SECTION("Adding and Verifying Direct Calls Relationships") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls("Helper", "Logger");

        REQUIRE(calls_store.has_calls("Main", "Helper"));
        REQUIRE(calls_store.has_calls("Helper", "Logger"));
    }

    SECTION("Verifying Absence of Non-existent Direct Calls Relationships") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls("Helper", "Logger");

        REQUIRE_FALSE(calls_store.has_calls("Main", "Logger"));
        REQUIRE_FALSE(calls_store.has_calls("Helper", "Main"));
        REQUIRE_FALSE(calls_store.has_calls("Logger", "Helper"));
    }

    SECTION("Get All Direct Calls Relationships") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls("Main", "Logger");
        calls_store.add_calls("Helper", "Validator");

        auto allCalls = calls_store.get_all_calls();

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
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls("Main", "Utils");

        auto callees = calls_store.get_all_calls_values();

        REQUIRE(callees.size() == 2);
        REQUIRE(callees.find("Main") == callees.end());
        REQUIRE(callees.find("Helper") != callees.end());
        REQUIRE(callees.find("Utils") != callees.end());
    }

    SECTION("Retrieving All Callers") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls("Utils", "Logger");

        auto callers = calls_store.get_all_calls_keys();

        REQUIRE(callers.size() == 2);
        REQUIRE(callers.find("Main") != callers.end());
        REQUIRE(callers.find("Utils") != callers.end());
        REQUIRE(callers.find("Helper") == callers.end());
        REQUIRE(callers.find("Logger") == callers.end());
    }

    SECTION("Retrieving All Callees for a Given Caller") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls("Main", "Logger");

        auto callees = calls_store.get_callees("Main");

        REQUIRE(callees.size() == 2);
        REQUIRE(callees.find("Helper") != callees.end());
        REQUIRE(callees.find("Logger") != callees.end());
        REQUIRE(callees.find("Main") == callees.end());
    }

    SECTION("Retrieving Callees for Non-existent Caller Returns Empty Set") {
        auto callees = calls_store.get_callees("NonExisting");

        REQUIRE(callees.empty());
    }

    SECTION("Retrieving All Callers for a Given Callee") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls("Logger", "Helper");

        auto callers = calls_store.get_callers("Helper");

        REQUIRE(callers.size() == 2);
        REQUIRE(callers.find("Main") != callers.end());
        REQUIRE(callers.find("Logger") != callers.end());
        REQUIRE(callers.find("Helper") == callers.end());
    }

    SECTION("Retrieving Callers for Non-existent Callee Returns Empty Set") {
        auto callers = calls_store.get_callers("NonExisting");

        REQUIRE(callers.empty());
    }

    SECTION("Adding and Verifying Calls* Relationships") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls_star("Main", "Utils");

        REQUIRE(calls_store.has_calls_star("Main", "Helper"));
        REQUIRE(calls_store.has_calls_star("Main", "Utils"));
    }

    SECTION("Verifying Absence of Non-existent Calls* Relationships") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls_star("Main", "Utils");

        REQUIRE_FALSE(calls_store.has_calls_star("Utils", "Main"));
        REQUIRE_FALSE(calls_store.has_calls_star("Helper", "Main"));
        REQUIRE_FALSE(calls_store.has_calls_star("Helper", "Utils"));
    }

    SECTION("Getting All Transitive Calls Relationships") {
        calls_store.add_calls("Main", "Helper");
        calls_store.add_calls("Helper", "Logger");
        calls_store.add_calls_star("Main", "Logger");

        auto allCallsStar = calls_store.get_all_calls_star();

        REQUIRE(allCallsStar.size() == 2);
        REQUIRE(allCallsStar["Main"].size() == 2);
        REQUIRE(allCallsStar["Main"].count("Helper") == 1);
        REQUIRE(allCallsStar["Main"].count("Logger") == 1);
        REQUIRE(allCallsStar["Helper"].count("Logger") == 1);

        REQUIRE(allCallsStar.count("Logger") == 0);
    }

    SECTION("Retrieving All Transitive Callees") {
        calls_store.add_calls_star("Main", "Utils");
        calls_store.add_calls_star("Main", "Logger");

        auto starCallees = calls_store.get_all_calls_star_values();

        REQUIRE(starCallees.size() == 2);
        REQUIRE(starCallees.find("Utils") != starCallees.end());
        REQUIRE(starCallees.find("Logger") != starCallees.end());
        REQUIRE(starCallees.find("Main") == starCallees.end());
    }

    SECTION("Retrieving All Transitive Callers") {
        calls_store.add_calls_star("Main", "Utils");
        calls_store.add_calls_star("Logger", "Utils");

        auto starCallers = calls_store.get_all_calls_star_keys();

        REQUIRE(starCallers.size() == 2);
        REQUIRE(starCallers.find("Main") != starCallers.end());
        REQUIRE(starCallers.find("Logger") != starCallers.end());
        REQUIRE(starCallers.find("Utils") == starCallers.end());
    }

    SECTION("Retrieving All Transitive Callees for a Given Caller") {
        calls_store.add_calls_star("Main", "Utils");

        auto starCallees = calls_store.get_calls_star_callees("Main");

        REQUIRE(starCallees.find("Utils") != starCallees.end());
        REQUIRE_FALSE(starCallees.find("Main") != starCallees.end());
    }

    SECTION("Retrieving Transitive Callees for Non-existent Caller Returns Empty Set") {
        auto starCallees = calls_store.get_calls_star_callees("NonExisting");

        REQUIRE(starCallees.empty());
    }

    SECTION("Retrieving All Transitive Callers for a Given Callee") {
        calls_store.add_calls_star("Main", "Utils");
        calls_store.add_calls_star("Helper", "Utils");

        auto starCallers = calls_store.get_calls_star_callers("Utils");

        REQUIRE(starCallers.size() == 2);
        REQUIRE(starCallers.find("Main") != starCallers.end());
        REQUIRE(starCallers.find("Helper") != starCallers.end());
        REQUIRE(starCallers.find("Utils") == starCallers.end());
    }

    SECTION("Retrieving Transitive Callers for Non-existent Callee Returns Empty Set") {
        auto starCallers = calls_store.get_calls_star_callers("NonExisting");

        REQUIRE(starCallers.empty());
    }
}
