#include <catch.hpp>

#include "pkb/stores/follows_store/direct_follows_store.h"

TEST_CASE("Direct FollowsStore Tests") {
    DirectFollowsStore direct_follows_store;

    SECTION("Adding and Verifying Follows relationships") {
        direct_follows_store.add("1", "2");
        direct_follows_store.add("2", "3");

        REQUIRE(direct_follows_store.contains_key_val_pair("1", "2"));
        REQUIRE(direct_follows_store.contains_key_val_pair("2", "3"));
    }

    SECTION("Verifying Absence of Non-existent Follows relationships") {
        direct_follows_store.add("1", "2");

        REQUIRE_FALSE(direct_follows_store.contains_key_val_pair("2", "1"));
        REQUIRE_FALSE(direct_follows_store.contains_key_val_pair("2", "3"));
    }

    SECTION("Retrieving all Follows relationships") {
        direct_follows_store.add("1", "2");
        direct_follows_store.add("3", "4");

        auto allFollows = direct_follows_store.get_all();

        REQUIRE(allFollows.size() == 2);
        REQUIRE(allFollows["1"] == "2");
        REQUIRE(allFollows["3"] == "4");
    }

    SECTION("Getting keys from all Follows relationships") {
        direct_follows_store.add("1", "2");
        direct_follows_store.add("3", "4");

        auto keys = direct_follows_store.get_all_keys();

        REQUIRE(keys.size() == 2);
        REQUIRE(keys.find("1") != keys.end());
        REQUIRE(keys.find("3") != keys.end());
    }

    SECTION("Getting values from all Follows relationships") {
        direct_follows_store.add("1", "2");
        direct_follows_store.add("3", "4");

        auto values = direct_follows_store.get_all_vals();

        REQUIRE(values.size() == 2);
        REQUIRE(values.find("2") != values.end());
        REQUIRE(values.find("4") != values.end());
    }

    SECTION("Retrieving following statements") {
        direct_follows_store.add("1", "2");

        REQUIRE(direct_follows_store.get_val_by_key("1") == "2");
        REQUIRE(direct_follows_store.get_val_by_key("2").empty());
    }

    SECTION("Retrieving preceded statements") {
        direct_follows_store.add("1", "2");

        REQUIRE(direct_follows_store.get_key_by_val("2") == "1");
        REQUIRE(direct_follows_store.get_key_by_val("1").empty());
    }
}