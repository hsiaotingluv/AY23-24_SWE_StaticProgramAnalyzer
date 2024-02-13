#include <catch.hpp>

#include "PKB/Stores/FollowsStore.h"

TEST_CASE("FollowsStore Tests") {
    FollowsStore followsStore;

    SECTION("Adding and Verifying Follows relationships") {
        followsStore.addFollows("1", "2");

        REQUIRE(followsStore.hasFollows("1", "2"));
    }

    SECTION("Verifying Absence of Non-existent Follows relationships") {
        followsStore.addFollows("1", "2");

        REQUIRE_FALSE(followsStore.hasFollows("2", "1"));
        REQUIRE_FALSE(followsStore.hasFollows("2", "3"));
    }

    SECTION("Retrieving all Follows relationships") {
        followsStore.addFollows("1", "2");
        followsStore.addFollows("3", "4");

        auto allFollows = followsStore.getAllFollows();

        REQUIRE(allFollows.size() == 2);
        REQUIRE(allFollows["1"] == "2");
        REQUIRE(allFollows["3"] == "4");
    }

    SECTION("Getting keys from all Follows relationships") {
        followsStore.addFollows("1", "2");
        followsStore.addFollows("3", "4");

        auto keys = followsStore.getAllFollowsKeys();

        REQUIRE(keys.size() == 2);
        REQUIRE(keys.find("1") != keys.end());
        REQUIRE(keys.find("3") != keys.end());
    }

    SECTION("Getting values from all Follows relationships") {
        followsStore.addFollows("1", "2");
        followsStore.addFollows("3", "4");

        auto values = followsStore.getAllFollowsValues();

        REQUIRE(values.size() == 2);
        REQUIRE(values.find("2") != values.end());
        REQUIRE(values.find("4") != values.end());
    }

    SECTION("Retrieving following statements") {
        followsStore.addFollows("1", "2");

        REQUIRE(followsStore.getFollowsFollowing("1") == "2");
        REQUIRE(followsStore.getFollowsFollowing("2").empty());
    }

    SECTION("Retrieving preceded statements") {
        followsStore.addFollows("1", "2");

        REQUIRE(followsStore.getFollowsBy("2") == "1");
        REQUIRE(followsStore.getFollowsBy("1").empty());
    }

    SECTION("Adding and Verifying Follows* relationships") {
        followsStore.addFollows("1", "2");
        followsStore.addFollowsStars("1", "3");

        REQUIRE(followsStore.hasFollowsStars("1", "2"));
        REQUIRE(followsStore.hasFollowsStars("1", "3"));
    }

    SECTION("Verifying Absence of Non-existent Follows* relationships") {
        followsStore.addFollows("1", "2");
        followsStore.addFollowsStars("1", "3");

        REQUIRE_FALSE(followsStore.hasFollowsStars("1", "4"));
        REQUIRE_FALSE(followsStore.hasFollowsStars("3", "1"));
    }

    SECTION("Retrieving all Follows* relationships") {
        followsStore.addFollowsStars("1", "2");
        followsStore.addFollowsStars("1", "3");

        auto allFollowsStar = followsStore.getAllFollowsStar();

        REQUIRE(allFollowsStar["1"].size() == 2);
        REQUIRE(allFollowsStar["1"].find("2") != allFollowsStar["1"].end());
        REQUIRE(allFollowsStar["1"].find("3") != allFollowsStar["1"].end());
    }

    SECTION("Getting keys from all Follows* relationships") {
        followsStore.addFollowsStars("1", "3");
        followsStore.addFollowsStars("2", "4");

        auto keys = followsStore.getAllFollowsStarKeys();

        REQUIRE(keys.size() == 2);
        REQUIRE(keys.find("1") != keys.end());
        REQUIRE(keys.find("2") != keys.end());
    }

    SECTION("Getting values from all Follows* relationships") {
        followsStore.addFollowsStars("1", "3");
        followsStore.addFollowsStars("2", "3");
        followsStore.addFollowsStars("2", "4");

        auto values = followsStore.getAllFollowsStarValues();

        REQUIRE(values.size() == 2);
        REQUIRE(values.find("3") != values.end());
        REQUIRE(values.find("4") != values.end());
    }

    SECTION("Retrieving statements followed by in Follows* relationships") {
        followsStore.addFollowsStars("1", "3");

        auto following = followsStore.getFollowsStarsFollowing("1");

        REQUIRE(!following.empty());
        REQUIRE(following.find("3") != following.end());

        auto noFollowing = followsStore.getFollowsStarsFollowing("3");

        REQUIRE(noFollowing.empty());
    }

    SECTION("Retrieving statements following in Follows* relationships") {
        followsStore.addFollowsStars("1", "3");

        auto by = followsStore.getFollowsStarsBy("3");

        REQUIRE(!by.empty());
        REQUIRE(by.find("1") != by.end());

        auto noBy = followsStore.getFollowsStarsBy("1");

        REQUIRE(noBy.empty());
    }
}
