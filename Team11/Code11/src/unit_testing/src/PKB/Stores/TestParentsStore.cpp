#include "PKB/Stores/ParentStore.h"
#include <catch.hpp>

TEST_CASE("ParentStore Tests") {
    ParentStore parentStore;

    SECTION("Adding and Verifying Direct Parent Relationships") {
        parentStore.addParent("1", "2");
        parentStore.addParent("1", "3");

        REQUIRE(parentStore.hasParent("1", "2"));
        REQUIRE(parentStore.hasParent("1", "3"));
    }

    SECTION("Verifying Absence of Non-existent Direct Parent Relationships") {
        parentStore.addParent("1", "2");
        parentStore.addParent("1", "3");

        REQUIRE_FALSE(parentStore.hasParent("2", "1"));
        REQUIRE_FALSE(parentStore.hasParent("3", "1"));
        REQUIRE_FALSE(parentStore.hasParent("1", "4"));
    }

    SECTION("Retrieving All Direct Parent Relationships") {
        parentStore.addParent("1", "2");
        parentStore.addParent("3", "4");

        auto allParents = parentStore.getAllParent();
        REQUIRE(allParents.size() == 2);
        REQUIRE(allParents["1"].find("2") != allParents["1"].end());
        REQUIRE(allParents["3"].find("4") != allParents["3"].end());
    }

    SECTION("Getting Parent Keys from All Parent Relationships") {
        parentStore.addParent("1", "2");
        parentStore.addParent("2", "3");

        auto parentKeys = parentStore.getAllParentKeys();
        REQUIRE(parentKeys.size() == 2);
        REQUIRE(parentKeys.find("1") != parentKeys.end());
        REQUIRE(parentKeys.find("2") != parentKeys.end());
        REQUIRE(parentKeys.find("3") == parentKeys.end());
    }

    SECTION("Getting Child Values from All Parent Relationships") {
        parentStore.addParent("1", "2");
        parentStore.addParent("2", "3");

        auto childValues = parentStore.getAllParentValues();
        REQUIRE(childValues.size() == 2);
        REQUIRE(childValues.find("1") == childValues.end());
        REQUIRE(childValues.find("2") != childValues.end());
        REQUIRE(childValues.find("3") != childValues.end());
    }

    SECTION("Retrieving Direct Children of a Specific Parent") {
        parentStore.addParent("1", "2");
        parentStore.addParent("1", "3");

        auto children = parentStore.getParentChildren("1");
        REQUIRE(children.size() == 2);
        REQUIRE(children.find("1") == children.end());
        REQUIRE(children.find("2") != children.end());
        REQUIRE(children.find("3") != children.end());
    }

    SECTION("Retrieving the Direct Parent of a Specific Child") {
        parentStore.addParent("1", "2");

        auto parent = parentStore.getParent("2");
        REQUIRE(parent == "1");
        REQUIRE_FALSE(parent == "2");
    }

    SECTION("Adding and Verifying Parent* Relationships") {
        parentStore.addParent("1", "2");
        parentStore.addParent("2", "3");
        parentStore.addParent("3", "4");
        parentStore.addParentStar("1", "3");

        REQUIRE(parentStore.hasParentStar("1", "2"));
        REQUIRE(parentStore.hasParentStar("1", "3"));
        REQUIRE(parentStore.hasParentStar("2", "3"));
        REQUIRE(parentStore.hasParentStar("3", "4"));
    }

    SECTION("Verifying Absence of Non-existent Parent* Relationships") {
        parentStore.addParent("1", "2");
        parentStore.addParent("2", "3");
        parentStore.addParent("3", "4");
        parentStore.addParentStar("1", "3");

        REQUIRE_FALSE(parentStore.hasParentStar("1", "1"));
        REQUIRE_FALSE(parentStore.hasParentStar("1", "4"));
        REQUIRE_FALSE(parentStore.hasParentStar("2", "1"));
        REQUIRE_FALSE(parentStore.hasParentStar("2", "2"));
        REQUIRE_FALSE(parentStore.hasParentStar("2", "4"));
        REQUIRE_FALSE(parentStore.hasParentStar("3", "1"));
        REQUIRE_FALSE(parentStore.hasParentStar("3", "2"));
        REQUIRE_FALSE(parentStore.hasParentStar("3", "3"));
    }

    SECTION("Retrieving All Parent* Relationships") {
        parentStore.addParentStar("1", "2");
        parentStore.addParentStar("1", "3");

        auto allParentStars = parentStore.getAllParentStar();
        REQUIRE(allParentStars.size() == 1);
        REQUIRE(allParentStars["1"].find("2") != allParentStars["1"].end());
        REQUIRE(allParentStars["1"].find("3") != allParentStars["1"].end());
    }

    SECTION("Getting Ancestor Keys from All Parent* Relationships") {
        parentStore.addParentStar("1", "3");
        parentStore.addParentStar("2", "3");

        auto parentStarKeys = parentStore.getAllParentStarKeys();
        REQUIRE(parentStarKeys.size() == 2);
        REQUIRE(parentStarKeys.find("1") != parentStarKeys.end());
        REQUIRE(parentStarKeys.find("2") != parentStarKeys.end());
        REQUIRE(parentStarKeys.find("3") == parentStarKeys.end());
    }

    SECTION("Getting Descendant Values from All Parent* Relationships") {
        parentStore.addParentStar("1", "2");
        parentStore.addParentStar("1", "3");

        auto parentStarValues = parentStore.getAllParentStarValues();
        REQUIRE(parentStarValues.size() == 2);
        REQUIRE(parentStarValues.find("1") == parentStarValues.end());
        REQUIRE(parentStarValues.find("2") != parentStarValues.end());
        REQUIRE(parentStarValues.find("3") != parentStarValues.end());
    }

    SECTION("Retrieving All Descendants of a Specific Ancestor in Parent* Relationships") {
        parentStore.addParentStar("1", "2");
        parentStore.addParentStar("1", "3");

        auto descendants = parentStore.getParentStarChildren("1");
        REQUIRE(descendants.size() == 2);
        REQUIRE(descendants.find("1") == descendants.end());
        REQUIRE(descendants.find("2") != descendants.end());
        REQUIRE(descendants.find("3") != descendants.end());
    }

    SECTION("Retrieving All Ancestors of a Specific Descendant in Parent* Relationships") {
        parentStore.addParentStar("1", "3");
        parentStore.addParentStar("2", "3");

        auto ancestors = parentStore.getStarParent("3");
        REQUIRE(ancestors.size() == 2);
        REQUIRE(ancestors.find("1") != ancestors.end());
        REQUIRE(ancestors.find("2") != ancestors.end());
        REQUIRE(ancestors.find("3") == ancestors.end());
    }
}
