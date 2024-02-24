#include <catch.hpp>

#include "pkb/stores/follows_store/follows_star_store.h"

TEST_CASE("Follows* Store Tests") {
    FollowsStarStore follows_star_store;

    SECTION("Adding and Verifying Transitive Follows* relationships") {
        follows_star_store.add("1", "2");
        follows_star_store.add("2", "3");
        follows_star_store.add("1", "3");

        REQUIRE(follows_star_store.contains_key_val_pair("1", "2"));
        REQUIRE(follows_star_store.contains_key_val_pair("2", "3"));
        REQUIRE(follows_star_store.contains_key_val_pair("1", "3"));
    }
}
