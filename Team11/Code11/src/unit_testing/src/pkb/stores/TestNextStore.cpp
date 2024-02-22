#include <catch.hpp>

#include "pkb/stores/next_store.h"

TEST_CASE("NextStore Tests") {
    NextStore next_store;

    SECTION("Add and Verify Direct Next Relationship") {
        next_store.add_next("1", "2");
        next_store.add_next("2", "3");

        REQUIRE(next_store.has_next("1", "2"));
        REQUIRE(next_store.has_next("2", "3"));
        REQUIRE_FALSE(next_store.has_next("1", "3"));
    }

    SECTION("Verify Absence of Non-existent Direct Next Relationship") {
        next_store.add_next("1", "2");
        next_store.add_next("2", "3");

        REQUIRE_FALSE(next_store.has_next("2", "1"));
        REQUIRE_FALSE(next_store.has_next("3", "2"));
        REQUIRE_FALSE(next_store.has_next("1", "3"));
    }

    SECTION("Get All Direct Next Relationships") {
        next_store.add_next("1", "2");
        next_store.add_next("2", "3");
        next_store.add_next("2", "4");

        auto allNext = next_store.get_all_next();

        REQUIRE(allNext.size() == 2);
        REQUIRE(allNext["1"].find("1") == allNext["1"].end());
        REQUIRE(allNext["1"].find("2") != allNext["1"].end());
        REQUIRE(allNext["1"].find("3") == allNext["1"].end());
        REQUIRE(allNext["1"].find("4") == allNext["1"].end());
        REQUIRE(allNext["2"].find("1") == allNext["2"].end());
        REQUIRE(allNext["2"].find("2") == allNext["2"].end());
        REQUIRE(allNext["2"].find("3") != allNext["2"].end());
        REQUIRE(allNext["2"].find("4") != allNext["2"].end());
    }

    SECTION("Get Direct Next Successors") {
        next_store.add_next("1", "2");
        next_store.add_next("1", "3");

        auto next = next_store.get_next("1");

        REQUIRE(next.size() == 2);
        REQUIRE(next.find("1") == next.end());
        REQUIRE(next.find("2") != next.end());
        REQUIRE(next.find("3") != next.end());
    }

    SECTION("Get Direct Next Predecessors") {
        next_store.add_next("1", "2");
        next_store.add_next("3", "2");

        auto previous = next_store.get_next_previous("2");

        REQUIRE(previous.size() == 2);
        REQUIRE(previous.find("1") != previous.end());
        REQUIRE(previous.find("2") == previous.end());
        REQUIRE(previous.find("3") != previous.end());
    }

    SECTION("Add and Verify Transitive Next* Relationship") {
        next_store.add_next("1", "2");
        next_store.add_next_star("1", "3");

        REQUIRE(next_store.has_next_star("1", "2"));
        REQUIRE(next_store.has_next_star("1", "3"));
    }

    SECTION("Verify Absence of Non-existent Transitive Next* Relationship") {
        next_store.add_next("1", "2");
        next_store.add_next_star("1", "3");

        REQUIRE_FALSE(next_store.has_next_star("2", "1"));
        REQUIRE_FALSE(next_store.has_next_star("2", "3"));
        REQUIRE_FALSE(next_store.has_next_star("3", "1"));
    }

    SECTION("Get All Transitive Next* Relationships") {
        next_store.add_next("1", "2");
        next_store.add_next_star("1", "3");
        next_store.add_next_star("2", "3");

        auto allNextStar = next_store.get_all_next_star();

        REQUIRE(allNextStar.size() == 2);
        REQUIRE(allNextStar["1"].find("3") != allNextStar["1"].end());
        REQUIRE(allNextStar["1"].find("2") != allNextStar["1"].end());
        REQUIRE(allNextStar["2"].find("3") != allNextStar["2"].end());
        REQUIRE(allNextStar["2"].find("1") == allNextStar["2"].end());
        REQUIRE(allNextStar["1"].find("4") == allNextStar["1"].end());
    }

    SECTION("Get Transitive Next* Successors") {
        next_store.add_next("1", "2");
        next_store.add_next_star("1", "3");

        auto nextStar = next_store.get_next_star("1");

        REQUIRE(nextStar.size() == 2);
        REQUIRE(nextStar.find("2") != nextStar.end());
        REQUIRE(nextStar.find("3") != nextStar.end());
        REQUIRE_FALSE(nextStar.find("4") != nextStar.end());
    }

    SECTION("Test Get Transitive Next* Predecessors") {
        next_store.add_next("2", "3");
        next_store.add_next_star("1", "3");

        auto previousStar = next_store.get_next_star_previous("3");

        REQUIRE(previousStar.size() == 2);
        REQUIRE(previousStar.find("1") != previousStar.end());
        REQUIRE(previousStar.find("2") != previousStar.end());
        REQUIRE_FALSE(previousStar.find("4") != previousStar.end());
    }
}
