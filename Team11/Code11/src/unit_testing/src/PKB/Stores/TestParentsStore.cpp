#include "pkb/stores/parent_store.h"
#include <catch.hpp>

TEST_CASE("ParentStore Tests") {
    ParentStore parent_store;

    SECTION("Adding and Verifying Direct Parent Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("1", "3");

        REQUIRE(parent_store.has_parent("1", "2"));
        REQUIRE(parent_store.has_parent("1", "3"));
    }

    SECTION("Verifying Absence of Non-existent Direct Parent Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("1", "3");

        REQUIRE_FALSE(parent_store.has_parent("2", "1"));
        REQUIRE_FALSE(parent_store.has_parent("3", "1"));
        REQUIRE_FALSE(parent_store.has_parent("1", "4"));
    }

    SECTION("Retrieving All Direct Parent Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("3", "4");

        auto allParents = parent_store.get_all_parent();

        REQUIRE(allParents.size() == 2);
        REQUIRE(allParents["1"].find("2") != allParents["1"].end());
        REQUIRE(allParents["3"].find("4") != allParents["3"].end());
    }

    SECTION("Getting Parent Keys from All Parent Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("2", "3");

        auto parentKeys = parent_store.get_all_parent_keys();

        REQUIRE(parentKeys.size() == 2);
        REQUIRE(parentKeys.find("1") != parentKeys.end());
        REQUIRE(parentKeys.find("2") != parentKeys.end());
        REQUIRE(parentKeys.find("3") == parentKeys.end());
    }

    SECTION("Getting Child Values from All Parent Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("2", "3");

        auto childValues = parent_store.get_all_parent_values();

        REQUIRE(childValues.size() == 2);
        REQUIRE(childValues.find("1") == childValues.end());
        REQUIRE(childValues.find("2") != childValues.end());
        REQUIRE(childValues.find("3") != childValues.end());
    }

    SECTION("Retrieving Direct Children of a Specific Parent") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("1", "3");

        auto children = parent_store.get_parent_children("1");

        REQUIRE(children.size() == 2);
        REQUIRE(children.find("1") == children.end());
        REQUIRE(children.find("2") != children.end());
        REQUIRE(children.find("3") != children.end());
    }

    SECTION("Retrieving the Direct Parent of a Specific Child") {
        parent_store.add_parent("1", "2");

        auto parent = parent_store.get_parent("2");

        REQUIRE(parent == "1");
        REQUIRE_FALSE(parent == "2");
    }

    SECTION("Adding and Verifying Parent* Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("2", "3");
        parent_store.add_parent("3", "4");
        parent_store.populate_parent_and_reverse_parent_star();

        REQUIRE(parent_store.has_parent_star("1", "2"));
        REQUIRE(parent_store.has_parent_star("1", "3"));
        REQUIRE(parent_store.has_parent_star("1", "4"));
        REQUIRE(parent_store.has_parent_star("2", "3"));
        REQUIRE(parent_store.has_parent_star("2", "4"));
        REQUIRE(parent_store.has_parent_star("3", "4"));
    }

    SECTION("Verifying Absence of Non-existent Parent* Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("2", "3");
        parent_store.add_parent("3", "4");
        parent_store.populate_parent_and_reverse_parent_star();

        REQUIRE_FALSE(parent_store.has_parent_star("1", "1"));
        REQUIRE_FALSE(parent_store.has_parent_star("2", "1"));
        REQUIRE_FALSE(parent_store.has_parent_star("2", "2"));
        REQUIRE_FALSE(parent_store.has_parent_star("3", "1"));
        REQUIRE_FALSE(parent_store.has_parent_star("3", "2"));
        REQUIRE_FALSE(parent_store.has_parent_star("3", "3"));
    }

    SECTION("Retrieving All Parent* Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("1", "3");
        parent_store.populate_parent_and_reverse_parent_star();

        auto allParentStars = parent_store.get_all_parent_star();

        REQUIRE(allParentStars.size() == 1);
        REQUIRE(allParentStars["1"].find("2") != allParentStars["1"].end());
        REQUIRE(allParentStars["1"].find("3") != allParentStars["1"].end());
    }

    SECTION("Getting Ancestor Keys from All Parent* Relationships") {
        parent_store.add_parent("1", "3");
        parent_store.add_parent("2", "3");
        parent_store.populate_parent_and_reverse_parent_star();

        auto parentStarKeys = parent_store.get_all_parent_star_keys();

        REQUIRE(parentStarKeys.size() == 2);
        REQUIRE(parentStarKeys.find("1") != parentStarKeys.end());
        REQUIRE(parentStarKeys.find("2") != parentStarKeys.end());
    }

    SECTION("Getting Descendant Values from All Parent* Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("1", "3");
        parent_store.populate_parent_and_reverse_parent_star();

        auto parentStarValues = parent_store.get_all_parent_star_values();

        REQUIRE(parentStarValues.size() == 2);
        REQUIRE(parentStarValues.find("1") == parentStarValues.end());
        REQUIRE(parentStarValues.find("2") != parentStarValues.end());
        REQUIRE(parentStarValues.find("3") != parentStarValues.end());
    }

    SECTION("Retrieving All Descendants of a Specific Ancestor in Parent* Relationships") {
        parent_store.add_parent("1", "2");
        parent_store.add_parent("1", "3");
        parent_store.populate_parent_and_reverse_parent_star();

        auto descendants = parent_store.get_parent_star_children("1");

        REQUIRE(descendants.size() == 2);
        REQUIRE(descendants.find("1") == descendants.end());
        REQUIRE(descendants.find("2") != descendants.end());
        REQUIRE(descendants.find("3") != descendants.end());
    }

    SECTION("Retrieving All Ancestors of a Specific Descendant in Parent* Relationships") {
        parent_store.add_parent("1", "3");
        parent_store.add_parent("2", "3");
        parent_store.populate_parent_and_reverse_parent_star();

        auto ancestors = parent_store.get_star_parent("3");

        REQUIRE(ancestors.size() == 2);
        REQUIRE(ancestors.find("1") != ancestors.end());
        REQUIRE(ancestors.find("2") != ancestors.end());
        REQUIRE(ancestors.find("3") == ancestors.end());
    }

    SECTION("Adding and Verifying Complex Follows* relationships") {
        parent_store.add_parent("4", "5");
        parent_store.add_parent("4", "6");
        parent_store.add_parent("6", "7");
        parent_store.add_parent("6", "8");
        parent_store.add_parent("6", "10");
        parent_store.add_parent("4", "11");
        parent_store.add_parent("4", "12");
        parent_store.add_parent("4", "13");
        parent_store.add_parent("8", "9");
        parent_store.populate_parent_and_reverse_parent_star();

        auto allParentStar = parent_store.get_all_parent_star();

        REQUIRE(parent_store.has_parent_star("4", "5"));
        REQUIRE(parent_store.has_parent_star("4", "6"));
        REQUIRE(parent_store.has_parent_star("4", "7"));
        REQUIRE(parent_store.has_parent_star("4", "8"));
        REQUIRE(parent_store.has_parent_star("4", "9"));
        REQUIRE(parent_store.has_parent_star("4", "10"));
        REQUIRE(parent_store.has_parent_star("4", "11"));
        REQUIRE(parent_store.has_parent_star("4", "12"));
        REQUIRE(parent_store.has_parent_star("4", "13"));

        REQUIRE(parent_store.has_parent_star("6", "7"));
        REQUIRE(parent_store.has_parent_star("6", "8"));
        REQUIRE(parent_store.has_parent_star("6", "9"));
        REQUIRE(parent_store.has_parent_star("6", "10"));

        REQUIRE(parent_store.has_parent_star("8", "9"));
    }
}
