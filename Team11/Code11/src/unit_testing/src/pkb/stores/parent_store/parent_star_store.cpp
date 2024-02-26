#include <catch.hpp>

#include "pkb/stores/parent_store/parent_star_store.h"

TEST_CASE("Parent* Store Tests") {
    ParentStarStore parent_star_store;

    SECTION("Adding and Verifying Transitive Parent* relationships") {
        parent_star_store.add("1", "2");
        parent_star_store.add("2", "3");
        parent_star_store.add("3", "4");

        REQUIRE(parent_star_store.contains_key_val_pair("1", "2"));
        REQUIRE(parent_star_store.contains_key_val_pair("2", "3"));
        REQUIRE(parent_star_store.contains_key_val_pair("3", "4"));
    }
}
