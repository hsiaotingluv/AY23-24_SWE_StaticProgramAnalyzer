#include <catch.hpp>

#include "PKB/Stores/NextStore.h"

TEST_CASE("NextStore Tests") {
    NextStore nextStore;

    SECTION("Add and Verify Direct Next Relationship") {
        nextStore.addNext("1", "2");
        nextStore.addNext("2", "3");

        REQUIRE(nextStore.hasNext("1", "2"));
        REQUIRE(nextStore.hasNext("2", "3"));
        REQUIRE_FALSE(nextStore.hasNext("1", "3"));
    }

    SECTION("Verify Absence of Non-existent Direct Next Relationship") {
        nextStore.addNext("1", "2");
        nextStore.addNext("2", "3");

        REQUIRE_FALSE(nextStore.hasNext("2", "1"));
        REQUIRE_FALSE(nextStore.hasNext("3", "2"));
        REQUIRE_FALSE(nextStore.hasNext("1", "3"));
    }

    SECTION("Get All Direct Next Relationships") {
        nextStore.addNext("1", "2");
        nextStore.addNext("2", "3");
        nextStore.addNext("2", "4");

        auto allNext = nextStore.getAllNext();

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
        nextStore.addNext("1", "2");
        nextStore.addNext("1", "3");

        auto next = nextStore.getNext("1");

        REQUIRE(next.size() == 2);
        REQUIRE(next.find("1") == next.end());
        REQUIRE(next.find("2") != next.end());
        REQUIRE(next.find("3") != next.end());
    }

    SECTION("Get Direct Next Predecessors") {
        nextStore.addNext("1", "2");
        nextStore.addNext("3", "2");

        auto previous = nextStore.getNextPrevious("2");

        REQUIRE(previous.size() == 2);
        REQUIRE(previous.find("1") != previous.end());
        REQUIRE(previous.find("2") == previous.end());
        REQUIRE(previous.find("3") != previous.end());
    }

    SECTION("Add and Verify Transitive Next* Relationship") {
        nextStore.addNext("1", "2");
        nextStore.addNextStar("1", "3");

        REQUIRE(nextStore.hasNextStar("1", "2"));
        REQUIRE(nextStore.hasNextStar("1", "3"));
    }

    SECTION("Verify Absence of Non-existent Transitive Next* Relationship") {
        nextStore.addNext("1", "2");
        nextStore.addNextStar("1", "3");

        REQUIRE_FALSE(nextStore.hasNextStar("2", "1"));
        REQUIRE_FALSE(nextStore.hasNextStar("2", "3"));
        REQUIRE_FALSE(nextStore.hasNextStar("3", "1"));
    }

    SECTION("Get All Transitive Next* Relationships") {
        nextStore.addNext("1", "2");
        nextStore.addNextStar("1", "3");
        nextStore.addNextStar("2", "3");

        auto allNextStar = nextStore.getAllNextStar();

        REQUIRE(allNextStar.size() == 2);
        REQUIRE(allNextStar["1"].find("3") != allNextStar["1"].end());
        REQUIRE(allNextStar["1"].find("2") != allNextStar["1"].end());
        REQUIRE(allNextStar["2"].find("3") != allNextStar["2"].end());
        REQUIRE(allNextStar["2"].find("1") == allNextStar["2"].end());
        REQUIRE(allNextStar["1"].find("4") == allNextStar["1"].end());
    }

    SECTION("Get Transitive Next* Successors") {
        nextStore.addNext("1", "2");
        nextStore.addNextStar("1", "3");

        auto nextStar = nextStore.getNextStar("1");

        REQUIRE(nextStar.size() == 2);
        REQUIRE(nextStar.find("2") != nextStar.end());
        REQUIRE(nextStar.find("3") != nextStar.end());
        REQUIRE_FALSE(nextStar.find("4") != nextStar.end());
    }

    SECTION("Test Get Transitive Next* Predecessors") {
        nextStore.addNext("2", "3");
        nextStore.addNextStar("1", "3");

        auto previousStar = nextStore.getNextStarPrevious("3");

        REQUIRE(previousStar.size() == 2);
        REQUIRE(previousStar.find("1") != previousStar.end());
        REQUIRE(previousStar.find("2") != previousStar.end());
        REQUIRE_FALSE(previousStar.find("4") != previousStar.end());
    }
}
