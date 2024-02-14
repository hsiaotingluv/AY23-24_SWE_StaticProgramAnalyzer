#include <catch.hpp>

#include "pkb/stores/follows_store.h"

TEST_CASE("FollowsStore Tests") {
    FollowsStore follows_store;

    SECTION("Adding and Verifying Follows relationships") {
        follows_store.add_follows("1", "2");

        REQUIRE(follows_store.has_follows("1", "2"));
    }

    SECTION("Verifying Absence of Non-existent Follows relationships") {
        follows_store.add_follows("1", "2");

        REQUIRE_FALSE(follows_store.has_follows("2", "1"));
        REQUIRE_FALSE(follows_store.has_follows("2", "3"));
    }

    SECTION("Retrieving all Follows relationships") {
        follows_store.add_follows("1", "2");
        follows_store.add_follows("3", "4");

        auto allFollows = follows_store.get_all_follows();

        REQUIRE(allFollows.size() == 2);
        REQUIRE(allFollows["1"] == "2");
        REQUIRE(allFollows["3"] == "4");
    }

    SECTION("Getting keys from all Follows relationships") {
        follows_store.add_follows("1", "2");
        follows_store.add_follows("3", "4");

        auto keys = follows_store.get_all_follows_keys();

        REQUIRE(keys.size() == 2);
        REQUIRE(keys.find("1") != keys.end());
        REQUIRE(keys.find("3") != keys.end());
    }

    SECTION("Getting values from all Follows relationships") {
        follows_store.add_follows("1", "2");
        follows_store.add_follows("3", "4");

        auto values = follows_store.get_all_follows_values();

        REQUIRE(values.size() == 2);
        REQUIRE(values.find("2") != values.end());
        REQUIRE(values.find("4") != values.end());
    }

    SECTION("Retrieving following statements") {
        follows_store.add_follows("1", "2");

        REQUIRE(follows_store.get_follows_following("1") == "2");
        REQUIRE(follows_store.get_follows_following("2").empty());
    }

    SECTION("Retrieving preceded statements") {
        follows_store.add_follows("1", "2");

        REQUIRE(follows_store.get_follows_by("2") == "1");
        REQUIRE(follows_store.get_follows_by("1").empty());
    }

    SECTION("Adding and Verifying Follows* relationships") {
        follows_store.add_follows("1", "2");
        follows_store.add_follows_stars("1", "3");

        REQUIRE(follows_store.has_follows_stars("1", "2"));
        REQUIRE(follows_store.has_follows_stars("1", "3"));
    }

    SECTION("Verifying Absence of Non-existent Follows* relationships") {
        follows_store.add_follows("1", "2");
        follows_store.add_follows_stars("1", "3");

        REQUIRE_FALSE(follows_store.has_follows_stars("1", "4"));
        REQUIRE_FALSE(follows_store.has_follows_stars("3", "1"));
    }

    SECTION("Retrieving all Follows* relationships") {
        follows_store.add_follows_stars("1", "2");
        follows_store.add_follows_stars("1", "3");

        auto allFollowsStar = follows_store.get_all_follows_star();

        REQUIRE(allFollowsStar["1"].size() == 2);
        REQUIRE(allFollowsStar["1"].find("2") != allFollowsStar["1"].end());
        REQUIRE(allFollowsStar["1"].find("3") != allFollowsStar["1"].end());
    }

    SECTION("Getting keys from all Follows* relationships") {
        follows_store.add_follows_stars("1", "3");
        follows_store.add_follows_stars("2", "4");

        auto keys = follows_store.get_all_follows_star_keys();

        REQUIRE(keys.size() == 2);
        REQUIRE(keys.find("1") != keys.end());
        REQUIRE(keys.find("2") != keys.end());
    }

    SECTION("Getting values from all Follows* relationships") {
        follows_store.add_follows_stars("1", "3");
        follows_store.add_follows_stars("2", "3");
        follows_store.add_follows_stars("2", "4");

        auto values = follows_store.get_all_follows_star_values();

        REQUIRE(values.size() == 2);
        REQUIRE(values.find("3") != values.end());
        REQUIRE(values.find("4") != values.end());
    }

    SECTION("Retrieving statements followed by in Follows* relationships") {
        follows_store.add_follows_stars("1", "3");

        auto following = follows_store.get_follows_stars_following("1");

        REQUIRE(!following.empty());
        REQUIRE(following.find("3") != following.end());

        auto noFollowing = follows_store.get_follows_stars_following("3");

        REQUIRE(noFollowing.empty());
    }

    SECTION("Retrieving statements following in Follows* relationships") {
        follows_store.add_follows_stars("1", "3");

        auto by = follows_store.get_follows_stars_by("3");

        REQUIRE(!by.empty());
        REQUIRE(by.find("1") != by.end());

        auto noBy = follows_store.get_follows_stars_by("1");

        REQUIRE(noBy.empty());
    }
}
