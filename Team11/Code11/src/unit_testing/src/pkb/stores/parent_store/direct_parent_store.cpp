#include "pkb/stores/parent_store/direct_parent_store.h"
#include <catch.hpp>

TEST_CASE("ParentStore Tests") {
    DirectParentStore direct_parent_store;

    SECTION("Adding and Verifying Direct Parent Relationships") {
        direct_parent_store.add("1", "2");
        direct_parent_store.add("1", "3");

        REQUIRE(direct_parent_store.contains_key_val_pair("1", "2"));
        REQUIRE(direct_parent_store.contains_key_val_pair("1", "3"));
    }

    SECTION("Verifying Absence of Non-existent Direct Parent Relationships") {
        direct_parent_store.add("1", "2");
        direct_parent_store.add("1", "3");

        REQUIRE_FALSE(direct_parent_store.contains_key_val_pair("2", "1"));
        REQUIRE_FALSE(direct_parent_store.contains_key_val_pair("3", "1"));
        REQUIRE_FALSE(direct_parent_store.contains_key_val_pair("1", "4"));
    }

    SECTION("Retrieving All Direct Parent Relationships") {
        direct_parent_store.add("1", "2");
        direct_parent_store.add("3", "4");

        auto allParents = direct_parent_store.get_all();

        REQUIRE(allParents.size() == 2);
        REQUIRE(allParents["1"].find("2") != allParents["1"].end());
        REQUIRE(allParents["3"].find("4") != allParents["3"].end());
    }

    SECTION("Getting Parent Keys from All Parent Relationships") {
        direct_parent_store.add("1", "2");
        direct_parent_store.add("2", "3");

        auto parentKeys = direct_parent_store.get_all_keys();

        REQUIRE(parentKeys.size() == 2);
        REQUIRE(parentKeys.find("1") != parentKeys.end());
        REQUIRE(parentKeys.find("2") != parentKeys.end());
        REQUIRE(parentKeys.find("3") == parentKeys.end());
    }

    SECTION("Getting Child Values from All Parent Relationships") {
        direct_parent_store.add("1", "2");
        direct_parent_store.add("2", "3");

        auto childValues = direct_parent_store.get_all_vals();

        REQUIRE(childValues.size() == 2);
        REQUIRE(childValues.find("1") == childValues.end());
        REQUIRE(childValues.find("2") != childValues.end());
        REQUIRE(childValues.find("3") != childValues.end());
    }

    SECTION("Retrieving Direct Children of a Specific Parent") {
        direct_parent_store.add("1", "2");
        direct_parent_store.add("1", "3");

        auto children = direct_parent_store.get_vals_by_key("1");

        REQUIRE(children.size() == 2);
        REQUIRE(children.find("1") == children.end());
        REQUIRE(children.find("2") != children.end());
        REQUIRE(children.find("3") != children.end());
    }

    SECTION("Retrieving Direct Parents of a Specific Child") {
        direct_parent_store.add("1", "2");

        auto parent = direct_parent_store.get_key_by_val("2");

        REQUIRE(parent == "1");
        REQUIRE_FALSE(parent == "2");
    }
}