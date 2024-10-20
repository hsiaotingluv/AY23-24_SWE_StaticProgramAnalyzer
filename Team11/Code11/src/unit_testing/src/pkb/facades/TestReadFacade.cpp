#include <catch.hpp>

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"

using namespace pkb;

TEST_CASE("Simple Entity Test") {
    SECTION("Get Entity Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_procedure("procedure1");

        REQUIRE(read_facade->get_procedures().size() == 1);
        REQUIRE(read_facade->get_variables().empty());
        REQUIRE(read_facade->get_entities().size() == 1);
    }

    SECTION("Entity Exists Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_procedure("main");
        write_facade->add_variable("Main");
        write_facade->add_constant("1");

        REQUIRE(read_facade->has_entity("main"));
        REQUIRE(read_facade->has_entity("Main"));
        REQUIRE(read_facade->has_entity("1"));

        REQUIRE_FALSE(read_facade->has_entity("2"));
        REQUIRE_FALSE(read_facade->has_entity("does_not_exists"));
    }
}

TEST_CASE("Simple Statement Test") {
    SECTION("Statement Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Print);
        write_facade->add_statement("3", StatementType::Assign);
        write_facade->add_statement("4", StatementType::If);
        write_facade->add_statement("5", StatementType::While);
        write_facade->add_statement("6", StatementType::Call);

        REQUIRE(read_facade->get_all_statements().size() == 6);
        REQUIRE(read_facade->get_read_statements().size() == 1);
        REQUIRE(read_facade->get_assign_statements().size() == 1);
        REQUIRE(read_facade->get_print_statements().size() == 1);
        REQUIRE(read_facade->get_if_statements().size() == 1);
        REQUIRE(read_facade->get_while_statements().size() == 1);
        REQUIRE(read_facade->get_call_statements().size() == 1);
    }

    SECTION("Statement Exists Tests") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Print);
        write_facade->add_statement("3", StatementType::Assign);
        write_facade->add_statement("4", StatementType::If);
        write_facade->add_statement("5", StatementType::While);
        write_facade->add_statement("6", StatementType::Call);

        REQUIRE(read_facade->has_read_statement("1"));
        REQUIRE(read_facade->has_print_statement("2"));
        REQUIRE(read_facade->has_assign_statement("3"));
        REQUIRE(read_facade->has_if_statement("4"));
        REQUIRE(read_facade->has_while_statement("5"));
        REQUIRE(read_facade->has_call_statement("6"));

        REQUIRE_FALSE(read_facade->has_read_statement("2"));
        REQUIRE_FALSE(read_facade->has_print_statement("3"));
        REQUIRE_FALSE(read_facade->has_assign_statement("4"));
        REQUIRE_FALSE(read_facade->has_if_statement("5"));
        REQUIRE_FALSE(read_facade->has_while_statement("6"));
        REQUIRE_FALSE(read_facade->has_call_statement("1"));
    }
}

TEST_CASE("Follows and FollowsStar Relationship Test") {
    SECTION("Follows and FollowsStar Transitive Relationship Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->add_follows("4", "5");
        write_facade->finalise_pkb();

        REQUIRE(read_facade->contains_follows_key("1"));
        REQUIRE(read_facade->contains_follows_key("2"));
        REQUIRE(read_facade->contains_follows_key("3"));
        REQUIRE(read_facade->contains_follows_key("4"));

        REQUIRE(read_facade->contains_follows_value("2"));
        REQUIRE(read_facade->contains_follows_value("3"));
        REQUIRE(read_facade->contains_follows_value("4"));
        REQUIRE(read_facade->contains_follows_value("5"));

        REQUIRE(read_facade->has_follows_relation());
        REQUIRE(read_facade->has_follows_relation("1", "2"));
        REQUIRE(read_facade->has_follows_relation("2", "3"));
        REQUIRE(read_facade->has_follows_relation("3", "4"));
        REQUIRE(read_facade->has_follows_relation("4", "5"));

        REQUIRE(read_facade->contains_follows_star_key("1"));
        REQUIRE(read_facade->contains_follows_star_key("2"));
        REQUIRE(read_facade->contains_follows_star_key("3"));
        REQUIRE(read_facade->contains_follows_star_key("4"));

        REQUIRE(read_facade->contains_follows_star_value("2"));
        REQUIRE(read_facade->contains_follows_star_value("3"));
        REQUIRE(read_facade->contains_follows_star_value("4"));
        REQUIRE(read_facade->contains_follows_star_value("5"));

        REQUIRE(read_facade->has_follows_star_relation());
        REQUIRE(read_facade->has_follows_star_relation("1", "2"));
        REQUIRE(read_facade->has_follows_star_relation("1", "3"));
        REQUIRE(read_facade->has_follows_star_relation("1", "4"));
        REQUIRE(read_facade->has_follows_star_relation("1", "5"));
        REQUIRE(read_facade->has_follows_star_relation("2", "3"));
        REQUIRE(read_facade->has_follows_star_relation("2", "4"));
        REQUIRE(read_facade->has_follows_star_relation("2", "5"));
        REQUIRE(read_facade->has_follows_star_relation("3", "4"));
        REQUIRE(read_facade->has_follows_star_relation("3", "5"));
        REQUIRE(read_facade->has_follows_star_relation("4", "5"));

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_follows_key("6"));
        REQUIRE_FALSE(read_facade->contains_follows_value("1"));

        REQUIRE_FALSE(read_facade->contains_follows_star_key("6"));
        REQUIRE_FALSE(read_facade->contains_follows_star_value("1"));
    }

    SECTION("Get All Followee By Statement Type Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->add_follows("4", "5");
        write_facade->add_follows("5", "6");
        write_facade->add_follows("6", "7");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Print);
        write_facade->add_statement("3", StatementType::While);
        write_facade->add_statement("4", StatementType::Assign);
        write_facade->add_statement("5", StatementType::If);
        write_facade->add_statement("6", StatementType::Call);
        write_facade->add_statement("7", StatementType::Read);

        REQUIRE(read_facade->get_all_follows_keys(StatementType::Read).size() == 1);
        REQUIRE(read_facade->get_all_follows_keys(StatementType::Print).size() == 1);
        REQUIRE(read_facade->get_all_follows_keys(StatementType::While).size() == 1);
        REQUIRE(read_facade->get_all_follows_star_keys(StatementType::Assign).size() == 1);
        REQUIRE(read_facade->get_all_follows_star_keys(StatementType::If).size() == 1);
        REQUIRE(read_facade->get_all_follows_star_keys(StatementType::Call).size() == 1);
    }

    SECTION("Get All Followers By Statement Type Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->add_follows("4", "5");
        write_facade->add_follows("5", "6");
        write_facade->add_follows("6", "7");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Print);
        write_facade->add_statement("3", StatementType::While);
        write_facade->add_statement("4", StatementType::Assign);
        write_facade->add_statement("5", StatementType::If);
        write_facade->add_statement("6", StatementType::Call);
        write_facade->add_statement("7", StatementType::Read);

        REQUIRE(read_facade->get_all_follows_values(StatementType::Read).size() == 1);
        REQUIRE(read_facade->get_all_follows_values(StatementType::Print).size() == 1);
        REQUIRE(read_facade->get_all_follows_values(StatementType::While).size() == 1);
        REQUIRE(read_facade->get_all_follows_star_values(StatementType::Assign).size() == 1);
        REQUIRE(read_facade->get_all_follows_star_values(StatementType::If).size() == 1);
        REQUIRE(read_facade->get_all_follows_star_values(StatementType::Call).size() == 1);
    }

    SECTION("Get All followers Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Print);
        write_facade->add_statement("3", StatementType::While);
        write_facade->add_statement("4", StatementType::While);

        REQUIRE(read_facade->get_statement_following("1") == "2");
        REQUIRE(read_facade->get_follows_stars_following("1").size() == 3);
        REQUIRE(read_facade->get_statement_following("1", StatementType::Read).empty());
        REQUIRE(read_facade->get_statement_following("1", StatementType::Print) == "2");
        REQUIRE(read_facade->get_follows_stars_following("1", StatementType::Read).empty());
        REQUIRE(read_facade->get_follows_stars_following("1", StatementType::While).size() == 2);
    }

    SECTION("Get All Followee by Follower Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::While);
        write_facade->add_statement("3", StatementType::While);
        write_facade->add_statement("4", StatementType::While);

        REQUIRE(read_facade->get_statement_followed_by("2") == "1");
        REQUIRE(read_facade->get_follows_stars_by("4").size() == 3);
        REQUIRE(read_facade->get_statement_followed_by("2", StatementType::Read) == "1");
        REQUIRE(read_facade->get_statement_followed_by("2", StatementType::Print).empty());
        REQUIRE(read_facade->get_follows_stars_by("4", StatementType::While).size() == 2);
        REQUIRE(read_facade->get_follows_stars_by("4", StatementType::If).empty());
    }

    SECTION("Simple follows star relationship") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->finalise_pkb();

        REQUIRE(read_facade->has_follows_star_relation("1", "2"));
        REQUIRE(read_facade->has_follows_star_relation("1", "3"));
        REQUIRE(read_facade->has_follows_star_relation("2", "3"));
    }

    SECTION("Verifying absence of non-existent Follows* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->finalise_pkb();

        REQUIRE_FALSE(read_facade->has_follows_star_relation("1", "4"));
        REQUIRE_FALSE(read_facade->has_follows_star_relation("3", "1"));
    }

    SECTION("Retrieving all Follows* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->finalise_pkb();

        auto all_keys = read_facade->get_all_follows_star_keys();
        REQUIRE(all_keys.size() == 2);
        REQUIRE(read_facade->get_follows_stars_following("1").size() == 2);
        REQUIRE(read_facade->get_follows_stars_following("2").size() == 1);
        REQUIRE(read_facade->get_follows_stars_following("3").empty());
    }

    SECTION("Getting keys from all follows* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->finalise_pkb();

        auto keys = read_facade->get_all_follows_star_keys();
        REQUIRE(keys.size() == 2);
        REQUIRE(keys.find("1") != keys.end());
        REQUIRE(keys.find("2") != keys.end());
    }

    SECTION("Getting values frm all Follows* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "3");
        write_facade->add_follows("2", "4");
        write_facade->finalise_pkb();

        auto values = read_facade->get_all_follows_star_values();
        REQUIRE(values.size() == 2);
        REQUIRE(values.find("3") != values.end());
        REQUIRE(values.find("4") != values.end());
    }

    SECTION("Retrieving statements followed by in Follows* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "3");
        write_facade->finalise_pkb();

        auto following = read_facade->get_follows_stars_following("1");

        REQUIRE(!following.empty());
        REQUIRE(following.find("3") != following.end());

        auto noFollowing = read_facade->get_follows_stars_following("3");

        REQUIRE(noFollowing.empty());
    }

    SECTION("Retrieving statements following in Follows* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "3");
        write_facade->finalise_pkb();

        auto by = read_facade->get_follows_stars_by("3");

        REQUIRE(!by.empty());
        REQUIRE(by.find("1") != by.end());

        auto noBy = read_facade->get_follows_stars_by("1");

        REQUIRE(noBy.empty());
    }

    SECTION("Adding and Verifying Complex Follows* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->add_follows("4", "12");
        write_facade->add_follows("5", "6");
        write_facade->add_follows("6", "9");
        write_facade->add_follows("9", "10");
        write_facade->add_follows("10", "11");
        write_facade->finalise_pkb();

        auto allFollowsStar = read_facade->get_all_follows_star();

        REQUIRE(allFollowsStar.size() == 8);
        REQUIRE(allFollowsStar["1"].find("2") != allFollowsStar["1"].end());
        REQUIRE(allFollowsStar["1"].find("3") != allFollowsStar["1"].end());
        REQUIRE(allFollowsStar["1"].find("4") != allFollowsStar["1"].end());
        REQUIRE(allFollowsStar["1"].find("12") != allFollowsStar["1"].end());

        REQUIRE(allFollowsStar["2"].find("3") != allFollowsStar["2"].end());
        REQUIRE(allFollowsStar["2"].find("4") != allFollowsStar["2"].end());
        REQUIRE(allFollowsStar["2"].find("12") != allFollowsStar["2"].end());

        REQUIRE(allFollowsStar["3"].find("4") != allFollowsStar["3"].end());
        REQUIRE(allFollowsStar["3"].find("12") != allFollowsStar["3"].end());

        REQUIRE(allFollowsStar["4"].find("12") != allFollowsStar["4"].end());

        REQUIRE(allFollowsStar["5"].find("6") != allFollowsStar["5"].end());
        REQUIRE(allFollowsStar["5"].find("9") != allFollowsStar["5"].end());
        REQUIRE(allFollowsStar["5"].find("10") != allFollowsStar["5"].end());
        REQUIRE(allFollowsStar["5"].find("11") != allFollowsStar["5"].end());
    }
}

TEST_CASE("Parent and ParentStar Relationship Test") {
    SECTION("Parent and ParentStar Transitive Relationship Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("1", "3");
        write_facade->add_parent("3", "4");
        write_facade->add_parent("3", "5");
        write_facade->add_parent("3", "6");
        write_facade->add_parent("6", "7");
        write_facade->finalise_pkb();

        REQUIRE(read_facade->contains_parent_key("1"));
        REQUIRE(read_facade->contains_parent_key("3"));
        REQUIRE(read_facade->contains_parent_key("6"));

        REQUIRE(read_facade->contains_parent_value("2"));
        REQUIRE(read_facade->contains_parent_value("3"));
        REQUIRE(read_facade->contains_parent_value("4"));
        REQUIRE(read_facade->contains_parent_value("5"));
        REQUIRE(read_facade->contains_parent_value("6"));
        REQUIRE(read_facade->contains_parent_value("7"));

        REQUIRE(read_facade->has_parent_relation());
        REQUIRE(read_facade->has_parent_relation("1", "2"));
        REQUIRE(read_facade->has_parent_relation("1", "3"));
        REQUIRE(read_facade->has_parent_relation("3", "4"));
        REQUIRE(read_facade->has_parent_relation("3", "5"));
        REQUIRE(read_facade->has_parent_relation("3", "6"));
        REQUIRE(read_facade->has_parent_relation("6", "7"));

        REQUIRE(read_facade->contains_parent_star_key("1"));
        REQUIRE(read_facade->contains_parent_star_key("3"));
        REQUIRE(read_facade->contains_parent_star_key("6"));

        REQUIRE(read_facade->contains_parent_star_value("2"));
        REQUIRE(read_facade->contains_parent_star_value("3"));
        REQUIRE(read_facade->contains_parent_star_value("4"));
        REQUIRE(read_facade->contains_parent_star_value("5"));
        REQUIRE(read_facade->contains_parent_star_value("6"));
        REQUIRE(read_facade->contains_parent_star_value("7"));

        REQUIRE(read_facade->has_parent_star_relation());
        REQUIRE(read_facade->has_parent_star_relation("1", "2"));
        REQUIRE(read_facade->has_parent_star_relation("1", "3"));
        REQUIRE(read_facade->has_parent_star_relation("1", "4"));
        REQUIRE(read_facade->has_parent_star_relation("1", "5"));
        REQUIRE(read_facade->has_parent_star_relation("1", "6"));
        REQUIRE(read_facade->has_parent_star_relation("1", "7"));

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_parent_key("2"));
        REQUIRE_FALSE(read_facade->contains_parent_value("1"));

        REQUIRE_FALSE(read_facade->contains_parent_star_key("4"));
        REQUIRE_FALSE(read_facade->contains_parent_star_value("1"));
    }

    SECTION("Get All Parents By Statement Type Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("2", "3");
        write_facade->add_parent("3", "4");
        write_facade->add_parent("4", "5");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::If);
        write_facade->add_statement("2", StatementType::While);
        write_facade->add_statement("3", StatementType::If);
        write_facade->add_statement("4", StatementType::If);
        write_facade->add_statement("5", StatementType::If);

        REQUIRE(read_facade->get_all_parent_keys(StatementType::If).size() == 3);
        REQUIRE(read_facade->get_all_parent_keys(StatementType::While).size() == 1);
        REQUIRE(read_facade->get_all_parent_star_keys(StatementType::If).size() == 3);
        REQUIRE(read_facade->get_all_parent_star_keys(StatementType::While).size() == 1);
    }

    SECTION("Get All Children By Statement Type Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("2", "3");
        write_facade->add_parent("3", "4");
        write_facade->add_parent("4", "5");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::If);
        write_facade->add_statement("2", StatementType::While);
        write_facade->add_statement("3", StatementType::If);
        write_facade->add_statement("4", StatementType::If);
        write_facade->add_statement("5", StatementType::While);

        REQUIRE(read_facade->get_all_parent_values(StatementType::If).size() == 2);
        REQUIRE(read_facade->get_all_parent_values(StatementType::While).size() == 2);
        REQUIRE(read_facade->get_all_parent_star_values(StatementType::If).size() == 2);
        REQUIRE(read_facade->get_all_parent_star_values(StatementType::While).size() == 2);
    }

    SECTION("Get All Children Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("2", "3");
        write_facade->add_parent("3", "4");
        write_facade->add_parent("4", "5");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::If);
        write_facade->add_statement("2", StatementType::While);
        write_facade->add_statement("3", StatementType::If);
        write_facade->add_statement("4", StatementType::If);
        write_facade->add_statement("5", StatementType::While);

        REQUIRE(read_facade->get_children_of("1").size() == 1);
        REQUIRE(read_facade->get_children_star_of("1").size() == 4);
        REQUIRE(read_facade->get_children_of("1", StatementType::While).size() == 1);
        REQUIRE(read_facade->get_children_of("1", StatementType::If).empty());
        REQUIRE(read_facade->get_children_star_of("1", StatementType::While).size() == 2);
        REQUIRE(read_facade->get_children_star_of("1", StatementType::If).size() == 2);
    }

    SECTION("Get All Parent by Children Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("2", "3");
        write_facade->add_parent("3", "4");
        write_facade->add_parent("4", "5");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::If);
        write_facade->add_statement("2", StatementType::While);
        write_facade->add_statement("3", StatementType::If);
        write_facade->add_statement("4", StatementType::If);
        write_facade->add_statement("5", StatementType::While);

        REQUIRE(read_facade->get_parent_of("2") == "1");
        REQUIRE(read_facade->get_parent_star_of("4").size() == 3);
        REQUIRE(read_facade->get_parent_of("2", StatementType::If) == "1");
        REQUIRE(read_facade->get_parent_of("2", StatementType::While).empty());
        REQUIRE(read_facade->get_parent_star_of("5", StatementType::If).size() == 3);
        REQUIRE(read_facade->get_parent_star_of("5", StatementType::While).size() == 1);
    }

    SECTION("Adding and Verifying Parent* Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("2", "3");
        write_facade->add_parent("3", "4");
        write_facade->finalise_pkb();

        REQUIRE(read_facade->has_parent_star_relation("1", "2"));
        REQUIRE(read_facade->has_parent_star_relation("1", "3"));
        REQUIRE(read_facade->has_parent_star_relation("1", "4"));
        REQUIRE(read_facade->has_parent_star_relation("2", "3"));
        REQUIRE(read_facade->has_parent_star_relation("2", "4"));
        REQUIRE(read_facade->has_parent_star_relation("3", "4"));
    }

    SECTION("Verifying Absence of Non-existent Parent* Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("2", "3");
        write_facade->add_parent("3", "4");
        write_facade->finalise_pkb();

        REQUIRE_FALSE(read_facade->has_parent_star_relation("1", "1"));
        REQUIRE_FALSE(read_facade->has_parent_star_relation("2", "1"));
        REQUIRE_FALSE(read_facade->has_parent_star_relation("2", "2"));
        REQUIRE_FALSE(read_facade->has_parent_star_relation("3", "1"));
        REQUIRE_FALSE(read_facade->has_parent_star_relation("3", "2"));
        REQUIRE_FALSE(read_facade->has_parent_star_relation("3", "3"));
    }

    SECTION("Retrieving all parent* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("2", "3");
        write_facade->add_parent("3", "4");
        write_facade->finalise_pkb();

        auto allParentStars = read_facade->get_all_parent_star();

        REQUIRE(allParentStars.size() == 3);
        REQUIRE(allParentStars["1"].find("2") != allParentStars["1"].end());
        REQUIRE(allParentStars["1"].find("3") != allParentStars["1"].end());
        REQUIRE(allParentStars["1"].find("4") != allParentStars["1"].end());
        REQUIRE(allParentStars["2"].find("3") != allParentStars["2"].end());
        REQUIRE(allParentStars["2"].find("4") != allParentStars["2"].end());
        REQUIRE(allParentStars["3"].find("4") != allParentStars["3"].end());
    }

    SECTION("Getting keys from all parent* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("2", "3");
        write_facade->add_parent("3", "4");
        write_facade->finalise_pkb();

        auto keys = read_facade->get_all_parent_star_keys();

        REQUIRE(keys.size() == 3);
        REQUIRE(keys.find("1") != keys.end());
        REQUIRE(keys.find("2") != keys.end());
        REQUIRE(keys.find("3") != keys.end());
    }

    SECTION("Getting Descendant Values from All Parent* Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("1", "3");
        write_facade->finalise_pkb();

        auto parentStarValues = read_facade->get_all_parent_star_values();

        REQUIRE(parentStarValues.size() == 2);
        REQUIRE(parentStarValues.find("1") == parentStarValues.end());
        REQUIRE(parentStarValues.find("2") != parentStarValues.end());
        REQUIRE(parentStarValues.find("3") != parentStarValues.end());
    }

    SECTION("Retrieving All Descendants of a Specific Ancestor in Parent* Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("1", "3");
        write_facade->finalise_pkb();

        auto descendants = read_facade->get_children_star_of("1");

        REQUIRE(descendants.size() == 2);
        REQUIRE(descendants.find("1") == descendants.end());
        REQUIRE(descendants.find("2") != descendants.end());
        REQUIRE(descendants.find("3") != descendants.end());
    }

    SECTION("Retrieving All Ancestors of a Specific Descendant in Parent* Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("1", "3");
        write_facade->add_parent("2", "3");
        write_facade->finalise_pkb();

        auto ancestors = read_facade->get_parent_star_of("3");

        REQUIRE(ancestors.size() == 2);
        REQUIRE(ancestors.find("1") != ancestors.end());
        REQUIRE(ancestors.find("2") != ancestors.end());
        REQUIRE(ancestors.find("3") == ancestors.end());
    }

    SECTION("Adding and Verifying Complex Parent* relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_parent("4", "5");
        write_facade->add_parent("4", "6");
        write_facade->add_parent("6", "7");
        write_facade->add_parent("6", "8");
        write_facade->add_parent("6", "10");
        write_facade->add_parent("4", "11");
        write_facade->add_parent("4", "12");
        write_facade->add_parent("4", "13");
        write_facade->add_parent("8", "9");
        write_facade->finalise_pkb();

        auto allParentStar = read_facade->get_all_parent_star();

        REQUIRE(read_facade->has_parent_star_relation("4", "5"));
        REQUIRE(read_facade->has_parent_star_relation("4", "6"));
        REQUIRE(read_facade->has_parent_star_relation("4", "7"));
        REQUIRE(read_facade->has_parent_star_relation("4", "8"));
        REQUIRE(read_facade->has_parent_star_relation("4", "9"));
        REQUIRE(read_facade->has_parent_star_relation("4", "10"));
        REQUIRE(read_facade->has_parent_star_relation("4", "11"));
        REQUIRE(read_facade->has_parent_star_relation("4", "12"));
        REQUIRE(read_facade->has_parent_star_relation("4", "13"));

        REQUIRE(read_facade->has_parent_star_relation("6", "7"));
        REQUIRE(read_facade->has_parent_star_relation("6", "8"));
        REQUIRE(read_facade->has_parent_star_relation("6", "9"));
        REQUIRE(read_facade->has_parent_star_relation("6", "10"));

        REQUIRE(read_facade->has_parent_star_relation("8", "9"));
    }
}

TEST_CASE("Modify Test") {
    SECTION("Contains Statement Modify Variable Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_statement_modify_var("1", "x");
        write_facade->add_statement_modify_var("1", "y");
        write_facade->add_statement_modify_var("2", "x");
        write_facade->add_statement_modify_var("2", "z");

        REQUIRE(read_facade->has_statement_modify_var());
        REQUIRE(read_facade->contains_statement_modify_var_key("1"));
        REQUIRE(read_facade->contains_statement_modify_var_key("2"));
        REQUIRE(read_facade->contains_statement_modify_var_value("x"));
        REQUIRE(read_facade->contains_statement_modify_var_value("y"));
        REQUIRE(read_facade->contains_statement_modify_var_value("z"));

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_statement_modify_var_key("nonexistent"));
        REQUIRE_FALSE(read_facade->contains_statement_modify_var_value("nonexistent"));
    }

    SECTION("Procedure Modify Variable Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_procedure_modify_var("main", "x");
        write_facade->add_procedure_modify_var("main", "y");
        write_facade->add_procedure_modify_var("helper", "z");

        REQUIRE(read_facade->has_procedure_modify_var());
        REQUIRE(read_facade->contains_procedure_modify_var_key("main"));
        REQUIRE(read_facade->contains_procedure_modify_var_key("helper"));
        REQUIRE(read_facade->contains_procedure_modify_var_value("x"));
        REQUIRE(read_facade->contains_procedure_modify_var_value("y"));
        REQUIRE(read_facade->contains_procedure_modify_var_value("z"));

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var_key("nonexistent"));
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var_value("nonexistent"));
    }

    SECTION("Simple Modify Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_statement_modify_var("1", "x");
        write_facade->add_statement_modify_var("1", "y");
        write_facade->add_statement_modify_var("2", "x");
        write_facade->add_statement_modify_var("2", "z");

        REQUIRE(read_facade->contains_statement_modify_var("1", "x"));
        REQUIRE(!read_facade->contains_statement_modify_var("1", "z"));
        REQUIRE(read_facade->get_statements_that_modify_var("x").size() == 2);
        REQUIRE(read_facade->get_statements_that_modify_var("z").size() == 1);
        REQUIRE(read_facade->get_vars_modified_by_statement("1").size() == 2);
    }

    SECTION("More complex modify test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_procedure_modify_var("main", "x");
        write_facade->add_procedure_modify_var("main", "y");
        write_facade->add_procedure_modify_var("helper", "z");
        write_facade->add_statement_modify_var("1", "x");
        write_facade->add_statement_modify_var("1", "y");
        write_facade->add_statement_modify_var("2", "x");
        write_facade->add_statement_modify_var("2", "z");

        REQUIRE(read_facade->get_all_statements_that_modify().size() == 2);
        REQUIRE(read_facade->contains_statement_modify_var_key("1"));
        REQUIRE(read_facade->get_all_statements_and_var_modify_pairs().size() == 4);
        REQUIRE(read_facade->get_all_procedures_that_modify().size() == 2);
        REQUIRE(read_facade->contains_procedure_modify_var_key("main"));
        REQUIRE(read_facade->get_all_procedures_and_var_modify_pairs().size() == 3);

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_statement_modify_var_key("3"));
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var_key("nonexistent"));
    }

    SECTION("Test get_statements_that_modify_var of certain type") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_statement_modify_var("1", "x");
        write_facade->add_statement_modify_var("1", "y");
        write_facade->add_statement_modify_var("2", "x");
        write_facade->add_statement_modify_var("2", "z");

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Call);

        REQUIRE(read_facade->get_statements_that_modify_var("x", StatementType::Read).size() == 1);
        REQUIRE(read_facade->get_statements_that_modify_var("z", StatementType::Call).size() == 1);
        REQUIRE(read_facade->get_statements_that_modify_var("x", StatementType::Print).empty());
        REQUIRE(read_facade->get_statements_that_modify_var("a", StatementType::If).empty());
        REQUIRE(read_facade->get_statements_that_modify_var("b", StatementType::Read).empty());
    }
}

TEST_CASE("Use Test") {
    SECTION("Contains Statement Use Variable Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_statement_use_var("1", "x");
        write_facade->add_statement_use_var("1", "y");
        write_facade->add_statement_use_var("2", "x");
        write_facade->add_statement_use_var("2", "z");

        REQUIRE(read_facade->has_statement_use_var());
        REQUIRE(read_facade->contains_statement_use_var_key("1"));
        REQUIRE(read_facade->contains_statement_use_var_key("2"));
        REQUIRE(read_facade->contains_statement_use_var_value("x"));
        REQUIRE(read_facade->contains_statement_use_var_value("y"));
        REQUIRE(read_facade->contains_statement_use_var_value("z"));

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_statement_use_var_key("nonexistent"));
        REQUIRE_FALSE(read_facade->contains_statement_use_var_value("nonexistent"));
    }

    SECTION("Contains Procedure Use Variable Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_procedure_use_var("main", "x");
        write_facade->add_procedure_use_var("main", "y");
        write_facade->add_procedure_use_var("helper", "z");

        REQUIRE(read_facade->has_procedure_use_var());
        REQUIRE(read_facade->contains_procedure_use_var_key("main"));
        REQUIRE(read_facade->contains_procedure_use_var_key("helper"));
        REQUIRE(read_facade->contains_procedure_use_var_value("x"));
        REQUIRE(read_facade->contains_procedure_use_var_value("y"));
        REQUIRE(read_facade->contains_procedure_use_var_value("z"));

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_procedure_use_var_key("nonexistent"));
        REQUIRE_FALSE(read_facade->contains_procedure_use_var_value("nonexistent"));
    }

    SECTION("Simple Use Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_statement_use_var("1", "x");
        write_facade->add_statement_use_var("1", "y");
        write_facade->add_statement_use_var("2", "x");
        write_facade->add_statement_use_var("2", "z");

        REQUIRE(read_facade->contains_statement_use_var("1", "x"));
        REQUIRE(!read_facade->contains_statement_use_var("1", "z"));
        REQUIRE(read_facade->get_statements_that_use_var("x").size() == 2);
        REQUIRE(read_facade->get_statements_that_use_var("z").size() == 1);
        REQUIRE(read_facade->get_vars_used_by_statement("1").size() == 2);
    }

    SECTION("More complex use test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_procedure_use_var("main", "x");
        write_facade->add_procedure_use_var("main", "y");
        write_facade->add_procedure_use_var("helper", "z");
        write_facade->add_statement_use_var("1", "x");
        write_facade->add_statement_use_var("1", "y");
        write_facade->add_statement_use_var("2", "x");
        write_facade->add_statement_use_var("2", "z");

        REQUIRE(read_facade->get_all_statements_that_use().size() == 2);
        REQUIRE(read_facade->contains_statement_use_var_key("1"));
        REQUIRE(read_facade->get_all_statements_and_var_use_pairs().size() == 4);
        REQUIRE(read_facade->get_all_procedures_that_use().size() == 2);
        REQUIRE(read_facade->contains_procedure_use_var_key("main"));
        REQUIRE(read_facade->get_all_procedures_and_var_use_pairs().size() == 3);

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_statement_use_var_key("3"));
        REQUIRE_FALSE(read_facade->contains_procedure_use_var_key("nonexistent"));
    }

    SECTION("Test get_statements_that_use_var of certain type") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_statement_use_var("1", "x");
        write_facade->add_statement_use_var("1", "y");
        write_facade->add_statement_use_var("2", "x");
        write_facade->add_statement_use_var("2", "z");

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Call);

        REQUIRE(read_facade->get_statements_that_use_var("x", StatementType::Read).size() == 1);
        REQUIRE(read_facade->get_statements_that_use_var("z", StatementType::Call).size() == 1);
        REQUIRE(read_facade->get_statements_that_use_var("x", StatementType::Print).empty());
        REQUIRE(read_facade->get_statements_that_use_var("a", StatementType::If).empty());
        REQUIRE(read_facade->get_statements_that_use_var("b", StatementType::Read).empty());
    }
}

TEST_CASE("Assignment Pattern Test") {
    SECTION("Simple Assignment Pattern Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_assignment("1", "x", "1 ");
        write_facade->add_assignment("2", "y", "x ");
        write_facade->add_assignment("3", "z", "2 ");
        write_facade->add_assignment("4", "q", "y ");
        write_facade->add_assignment("5", "x", "1 ");

        // Test exact rhs
        REQUIRE(read_facade->get_all_assignments_rhs("1 ").size() == 2);
        REQUIRE(read_facade->get_all_assignments_rhs("2 ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_rhs("x ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_rhs("y ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_rhs("z ").empty());

        // Test exact lhs
        REQUIRE(read_facade->get_all_assignments_lhs("x").size() == 2);
        REQUIRE(read_facade->get_all_assignments_lhs("y").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs("z").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs("q").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs("p").empty());

        // Test exact lhs and exact rhs
        REQUIRE(read_facade->get_all_assignments_lhs_rhs("x", "1 ").size() == 2);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs("y", "x ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs("z", "2 ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs("q", "y ").size() == 1);
    }

    SECTION("more complex assignment pattern test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_assignment("1", "x", "1 ");
        write_facade->add_assignment("2", "y", "x ");
        write_facade->add_assignment("3", "z", "2 ");
        write_facade->add_assignment("4", "q", "y ");
        write_facade->add_assignment("5", "x", "1 ");
        write_facade->add_assignment("6", "x", "2 ");
        write_facade->add_assignment("7", "xy", "3 4 + ");
        write_facade->add_assignment("8", "x", "1 y + ");
        write_facade->add_assignment("9", "xy", "y 1 + ");

        // Test partial RHS match, any lhs
        REQUIRE(read_facade->get_all_assignments_rhs_partial("1 ").size() == 4);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("2 ").size() == 2);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("x ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("y ").size() == 3);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("z ").empty());

        // Test partial RHS match, exact lhs
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("x", "1 ").size() == 3);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("y", "x ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("z", "2 ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("q", "y ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("x", "1 ").size() == 3);
    }
}

TEST_CASE("Calls and Calls* Relationship Test") {
    SECTION("Contains Calls and Calls* Relationships Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Logger");
        write_facade->finalise_pkb({"Logger", "Helper", "Main"});

        REQUIRE(read_facade->has_calls_relation());
        REQUIRE(read_facade->has_calls_star_relation());

        REQUIRE(read_facade->contains_calls_key("Main"));
        REQUIRE(read_facade->contains_calls_key("Helper"));

        REQUIRE(read_facade->contains_calls_value("Helper"));
        REQUIRE(read_facade->contains_calls_value("Logger"));

        REQUIRE(read_facade->contains_calls_star_key("Main"));
        REQUIRE(read_facade->contains_calls_star_key("Helper"));

        REQUIRE(read_facade->contains_calls_star_value("Helper"));
        REQUIRE(read_facade->contains_calls_star_value("Logger"));

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_calls_key("Logger"));
        REQUIRE_FALSE(read_facade->contains_calls_value("Main"));

        REQUIRE_FALSE(read_facade->contains_calls_star_key("Logger"));
        REQUIRE_FALSE(read_facade->contains_calls_star_value("Main"));
    }

    SECTION("Adding and Verifying Direct Calls Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Logger");

        REQUIRE(read_facade->has_calls_relation("Main", "Helper"));
        REQUIRE(read_facade->has_calls_relation("Helper", "Logger"));
    }

    SECTION("Verifying Absence of Non-existent Direct Calls Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Logger");

        REQUIRE_FALSE(read_facade->has_calls_relation("Main", "Logger"));
        REQUIRE_FALSE(read_facade->has_calls_relation("Helper", "Main"));
        REQUIRE_FALSE(read_facade->has_calls_relation("Logger", "Helper"));
    }

    SECTION("Retrieving All Direct Calls Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Main", "Logger");
        write_facade->add_calls("Helper", "Validator");

        auto calls_keys = read_facade->get_all_calls_callers();

        REQUIRE(calls_keys.size() == 2);

        auto main_calls = read_facade->get_callees("Main");

        REQUIRE(main_calls.size() == 2);
        REQUIRE(main_calls.count("Helper") == 1);
        REQUIRE(main_calls.count("Logger") == 1);

        auto helper_calls = read_facade->get_callees("Helper");

        REQUIRE(helper_calls.size() == 1);
        REQUIRE(helper_calls.count("Validator") == 1);

        auto logger_calls = read_facade->get_callees("Logger");
        auto validator_calls = read_facade->get_callees("Validator");

        REQUIRE(logger_calls.empty());
        REQUIRE(validator_calls.empty());
    }

    SECTION("Retrieving All Callees") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Main", "Utils");

        auto callees = read_facade->get_all_calls_callees();

        REQUIRE(callees.size() == 2);
        REQUIRE(callees.find("Main") == callees.end());
        REQUIRE(callees.find("Helper") != callees.end());
        REQUIRE(callees.find("Utils") != callees.end());
    }

    SECTION("Retrieving All Callers") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Utils", "Logger");

        auto callers = read_facade->get_all_calls_callers();

        REQUIRE(callers.size() == 2);
        REQUIRE(callers.find("Main") != callers.end());
        REQUIRE(callers.find("Utils") != callers.end());
        REQUIRE(callers.find("Helper") == callers.end());
        REQUIRE(callers.find("Logger") == callers.end());
    }

    SECTION("Retrieving All Callers for a Given Callee") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Logger", "Helper");

        auto callers = read_facade->get_callers("Helper");

        REQUIRE(callers.size() == 2);
        REQUIRE(callers.find("Main") != callers.end());
        REQUIRE(callers.find("Logger") != callers.end());
        REQUIRE(callers.find("Helper") == callers.end());
    }

    SECTION("Retrieving Callers for Non-existent Callee Returns Empty Set") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        auto callers = read_facade->get_callers("NonExisting");

        REQUIRE(callers.empty());
    }

    SECTION("Retrieving All Callees for a Given Caller") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Main", "Logger");

        auto callees = read_facade->get_callees("Main");

        REQUIRE(callees.size() == 2);
        REQUIRE(callees.find("Helper") != callees.end());
        REQUIRE(callees.find("Logger") != callees.end());
        REQUIRE(callees.find("Main") == callees.end());
    }

    SECTION("Retrieving Callees for Non-existent Caller Returns Empty Set") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        auto callees = read_facade->get_callees("NonExisting");

        REQUIRE(callees.empty());
    }

    SECTION("Adding and Verifying Calls* Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Logger");
        write_facade->finalise_pkb({"Logger", "Helper", "Main"});

        REQUIRE(read_facade->has_calls_star_relation("Main", "Helper"));
        REQUIRE(read_facade->has_calls_star_relation("Helper", "Logger"));
    }

    SECTION("Verifying Absence of Non-existent Calls* Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Logger");
        write_facade->finalise_pkb({"Logger", "Helper", "Main"});

        REQUIRE_FALSE(read_facade->has_calls_star_relation("Logger", "Helper"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("Main", "Main"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("Logger", "Main"));
    }

    SECTION("Retrieving All Calls* Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Main", "Logger");
        write_facade->add_calls("Helper", "Validator");
        write_facade->finalise_pkb({"Validator", "Helper", "Logger", "Main"});

        auto calls_keys = read_facade->get_all_calls_star_keys();

        REQUIRE(calls_keys.size() == 2);

        auto main_calls = read_facade->get_star_callees("Main");

        REQUIRE(main_calls.size() == 3);
        REQUIRE(main_calls.count("Helper") == 1);
        REQUIRE(main_calls.count("Logger") == 1);
        REQUIRE(main_calls.count("Validator") == 1);

        auto helper_calls = read_facade->get_star_callees("Helper");

        REQUIRE(helper_calls.size() == 1);
        REQUIRE(helper_calls.count("Validator") == 1);

        auto logger_calls = read_facade->get_star_callees("Logger");
        auto validator_calls = read_facade->get_star_callees("Validator");

        REQUIRE(logger_calls.empty());
        REQUIRE(validator_calls.empty());
    }

    SECTION("Retrieving All Calls* Callees") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Utils");
        write_facade->finalise_pkb({"Utils", "Helper", "Main"});

        auto callees = read_facade->get_all_calls_star_values();

        REQUIRE(callees.size() == 2);
        REQUIRE(callees.find("Main") == callees.end());
        REQUIRE(callees.find("Helper") != callees.end());
        REQUIRE(callees.find("Utils") != callees.end());
    }

    SECTION("Retrieving All Calls* Callers") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Logger");
        write_facade->finalise_pkb({"Logger", "Helper", "Main"});

        auto callers = read_facade->get_all_calls_star_keys();

        REQUIRE(callers.size() == 2);
        REQUIRE(callers.find("Main") != callers.end());
        REQUIRE(callers.find("Helper") != callers.end());
        REQUIRE(callers.find("Logger") == callers.end());
    }

    SECTION("Retrieving All Star Callers for a Given Callee") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Logger");
        write_facade->finalise_pkb({"Logger", "Helper", "Main"});

        auto callers = read_facade->get_star_callers("Logger");

        REQUIRE(callers.size() == 2);
        REQUIRE(callers.find("Main") != callers.end());
        REQUIRE(callers.find("Helper") != callers.end());
        REQUIRE(callers.find("Logger") == callers.end());
    }

    SECTION("More complex calls* relationship test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("A", "B");
        write_facade->add_calls("B", "C");
        write_facade->add_calls("C", "D");
        write_facade->add_calls("B", "D");
        write_facade->add_calls("D", "E");
        write_facade->add_calls("E", "F");
        write_facade->add_calls("D", "G");
        write_facade->add_calls("G", "H");
        write_facade->add_calls("H", "I");
        write_facade->add_calls("A", "I");
        write_facade->add_calls("H", "J");
        write_facade->add_calls("K", "J");

        write_facade->finalise_pkb({"J", "K", "I", "H", "G", "F", "E", "D", "C", "B", "A"});

        REQUIRE(read_facade->has_calls_star_relation("A", "B"));
        REQUIRE(read_facade->has_calls_star_relation("A", "C"));
        REQUIRE(read_facade->has_calls_star_relation("A", "D"));
        REQUIRE(read_facade->has_calls_star_relation("A", "E"));
        REQUIRE(read_facade->has_calls_star_relation("A", "F"));
        REQUIRE(read_facade->has_calls_star_relation("A", "G"));
        REQUIRE(read_facade->has_calls_star_relation("A", "H"));
        REQUIRE(read_facade->has_calls_star_relation("A", "I"));
        REQUIRE(read_facade->has_calls_star_relation("A", "J"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("A", "K"));
        REQUIRE(read_facade->has_calls_star_relation("B", "C"));
        REQUIRE(read_facade->has_calls_star_relation("B", "D"));
        REQUIRE(read_facade->has_calls_star_relation("B", "E"));
        REQUIRE(read_facade->has_calls_star_relation("B", "F"));
        REQUIRE(read_facade->has_calls_star_relation("B", "G"));
        REQUIRE(read_facade->has_calls_star_relation("B", "H"));
        REQUIRE(read_facade->has_calls_star_relation("B", "I"));
        REQUIRE(read_facade->has_calls_star_relation("B", "J"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("B", "K"));
        REQUIRE(read_facade->has_calls_star_relation("C", "D"));
        REQUIRE(read_facade->has_calls_star_relation("C", "E"));
        REQUIRE(read_facade->has_calls_star_relation("C", "F"));
        REQUIRE(read_facade->has_calls_star_relation("C", "G"));
        REQUIRE(read_facade->has_calls_star_relation("C", "H"));
        REQUIRE(read_facade->has_calls_star_relation("C", "I"));
        REQUIRE(read_facade->has_calls_star_relation("C", "J"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("C", "K"));
        REQUIRE(read_facade->has_calls_star_relation("D", "E"));
        REQUIRE(read_facade->has_calls_star_relation("D", "F"));
        REQUIRE(read_facade->has_calls_star_relation("D", "G"));
        REQUIRE(read_facade->has_calls_star_relation("D", "H"));
        REQUIRE(read_facade->has_calls_star_relation("D", "I"));
        REQUIRE(read_facade->has_calls_star_relation("D", "J"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("D", "K"));
        REQUIRE(read_facade->has_calls_star_relation("E", "F"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("E", "G"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("E", "H"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("E", "I"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("E", "J"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("E", "K"));
        REQUIRE_FALSE(read_facade->has_calls_star_relation("E", "D"));
        REQUIRE(read_facade->has_calls_star_relation("K", "J"));
    }

    SECTION("Retrieving Star Callers for Non-existent Callee Returns Empty Set") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        auto callers = read_facade->get_star_callers("NonExisting");

        REQUIRE(callers.empty());
    }

    SECTION("Retrieving All Star Callees for a Given Caller") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_calls("Main", "Helper");
        write_facade->add_calls("Helper", "Logger");
        write_facade->finalise_pkb({"Logger", "Helper", "Main"});

        auto callees = read_facade->get_star_callees("Main");

        REQUIRE(callees.size() == 2);
        REQUIRE(callees.find("Helper") != callees.end());
        REQUIRE(callees.find("Logger") != callees.end());
        REQUIRE(callees.find("Main") == callees.end());
    }

    SECTION("Retrieving Star Callees for Non-existent Caller Returns Empty Set") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        auto callees = read_facade->get_star_callees("NonExisting");

        REQUIRE(callees.empty());
    }
}

TEST_CASE("Statement number to procedure called mapping test") {
    SECTION("Simple statement number to procedure called mapping test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_stmt_no_proc_called_mapping("1", "Helper");
        write_facade->add_stmt_no_proc_called_mapping("2", "Helper");
        write_facade->add_stmt_no_proc_called_mapping("3", "Foobar");

        REQUIRE(read_facade->get_procedure_name_called_by("1") == "Helper");
        REQUIRE(read_facade->get_procedure_name_called_by("2") == "Helper");
        REQUIRE(read_facade->get_procedure_name_called_by("3") == "Foobar");
        REQUIRE(read_facade->get_procedure_name_called_by("4").empty());
    }
}

TEST_CASE("Next Test") {
    SECTION("Contains Next Relationship Test") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_next("1", "2");
        write_facade->add_next("2", "3");

        REQUIRE(read_facade->has_next_relation());

        REQUIRE(read_facade->contains_next_key("1"));
        REQUIRE(read_facade->contains_next_key("2"));

        REQUIRE(read_facade->contains_next_value("2"));
        REQUIRE(read_facade->contains_next_value("3"));

        // Negative testcases
        REQUIRE_FALSE(read_facade->contains_next_key("3"));
        REQUIRE_FALSE(read_facade->contains_next_value("1"));
    }

    SECTION("Add and Verify Direct Next Relationship") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_next("1", "2");
        write_facade->add_next("2", "3");

        REQUIRE(read_facade->has_next_relation("1", "2"));
        REQUIRE(read_facade->has_next_relation("2", "3"));
        REQUIRE_FALSE(read_facade->has_next_relation("1", "3"));
    }

    SECTION("Get All Direct Next Relationships") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_next("1", "2");
        write_facade->add_next("2", "3");
        write_facade->add_next("2", "4");

        auto next_keys = read_facade->get_all_next_keys();

        REQUIRE(next_keys.size() == 2);

        auto next_1 = read_facade->get_next_of("1");

        REQUIRE(next_1.size() == 1);
        REQUIRE(next_1.count("2") == 1);
        REQUIRE(next_1.count("1") == 0);
        REQUIRE(next_1.count("3") == 0);
        REQUIRE(next_1.count("4") == 0);

        auto next_2 = read_facade->get_next_of("2");

        REQUIRE(next_2.size() == 2);
        REQUIRE(next_2.count("2") == 0);
        REQUIRE(next_2.count("1") == 0);
        REQUIRE(next_2.count("3") == 1);
        REQUIRE(next_2.count("4") == 1);
    }

    SECTION("Get All Previous of a Statement") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_next("1", "2");
        write_facade->add_next("3", "2");
        write_facade->add_next("4", "2");

        auto previous = read_facade->get_previous_of("2");

        REQUIRE(previous.size() == 3);
        REQUIRE(previous.count("1") == 1);
        REQUIRE(previous.count("3") == 1);
        REQUIRE(previous.count("4") == 1);
        REQUIRE(previous.count("2") == 0);
    }

    SECTION("Get Direct Next Successors") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_next("1", "2");
        write_facade->add_next("1", "3");

        auto next = read_facade->get_next_of("1");

        REQUIRE(next.size() == 2);
        REQUIRE(next.count("1") == 0);
        REQUIRE(next.count("2") == 1);
        REQUIRE(next.count("3") == 1);
    }

    SECTION("Get Direct Next Predecessors") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_next("1", "2");
        write_facade->add_next("3", "2");

        auto previous = read_facade->get_previous_of("2");

        REQUIRE(previous.size() == 2);
        REQUIRE(previous.count("1") == 1);
        REQUIRE(previous.count("2") == 0);
        REQUIRE(previous.count("3") == 1);
    }
}

TEST_CASE("If Pattern Test") {
    SECTION("Get all if statements with any variables") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_if_var("1", "x");
        write_facade->add_if_var("2", "y");
        write_facade->add_if_var("3", "z");

        REQUIRE(read_facade->get_if_stmts_with_var().size() == 3);

        REQUIRE(read_facade->get_if_stmts_with_var().count("1") == 1);
        REQUIRE(read_facade->get_if_stmts_with_var().count("2") == 1);
        REQUIRE(read_facade->get_if_stmts_with_var().count("3") == 1);
        REQUIRE(read_facade->get_if_stmts_with_var().count("4") == 0);
    }

    SECTION("Get all if statement with specific variables") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_if_var("1", "x");
        write_facade->add_if_var("2", "y");
        write_facade->add_if_var("3", "z");

        REQUIRE(read_facade->get_if_stmts_with_var("x").size() == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("y").size() == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("z").size() == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("a").empty());

        REQUIRE(read_facade->get_if_stmts_with_var("x").count("1") == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("y").count("2") == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("z").count("3") == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("x").count("2") == 0);
    }

    SECTION("Get all variables inside ifs") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_if_var("1", "x");
        write_facade->add_if_var("1", "y");
        write_facade->add_if_var("2", "x");
        write_facade->add_if_var("2", "z");

        REQUIRE(read_facade->get_vars_in_any_if().size() == 3);

        REQUIRE(read_facade->get_vars_in_any_if().count("x") == 1);
        REQUIRE(read_facade->get_vars_in_any_if().count("y") == 1);
        REQUIRE(read_facade->get_vars_in_any_if().count("z") == 1);
        REQUIRE(read_facade->get_vars_in_any_if().count("a") == 0);
    }

    SECTION("Get all variables inside specific if") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_if_var("1", "x");
        write_facade->add_if_var("1", "y");
        write_facade->add_if_var("2", "x");
        write_facade->add_if_var("2", "z");

        REQUIRE(read_facade->get_vars_in_if("1").size() == 2);
        REQUIRE(read_facade->get_vars_in_if("2").size() == 2);
        REQUIRE(read_facade->get_vars_in_if("3").empty());

        REQUIRE(read_facade->get_vars_in_if("1").count("x") == 1);
        REQUIRE(read_facade->get_vars_in_if("1").count("y") == 1);
        REQUIRE(read_facade->get_vars_in_if("1").count("z") == 0);
        REQUIRE(read_facade->get_vars_in_if("2").count("x") == 1);
        REQUIRE(read_facade->get_vars_in_if("2").count("y") == 0);
        REQUIRE(read_facade->get_vars_in_if("2").count("z") == 1);
    }

    SECTION("Get all if statement and variable pairs") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_if_var("1", "x");
        write_facade->add_if_var("1", "y");
        write_facade->add_if_var("2", "x");
        write_facade->add_if_var("2", "z");

        REQUIRE(read_facade->get_all_if_stmt_var_pairs().size() == 4);
    }
}

TEST_CASE("While Pattern Test") {
    SECTION("Get all while statements with any variables") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_while_var("1", "x");
        write_facade->add_while_var("2", "y");
        write_facade->add_while_var("3", "z");

        REQUIRE(read_facade->get_while_stmts_with_var().size() == 3);

        REQUIRE(read_facade->get_while_stmts_with_var().count("1") == 1);
        REQUIRE(read_facade->get_while_stmts_with_var().count("2") == 1);
        REQUIRE(read_facade->get_while_stmts_with_var().count("3") == 1);
        REQUIRE(read_facade->get_while_stmts_with_var().count("4") == 0);
    }

    SECTION("Get all while statement with specific variables") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_while_var("1", "x");
        write_facade->add_while_var("2", "y");
        write_facade->add_while_var("3", "z");

        REQUIRE(read_facade->get_while_stmts_with_var("x").size() == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("y").size() == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("z").size() == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("a").empty());

        REQUIRE(read_facade->get_while_stmts_with_var("x").count("1") == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("y").count("2") == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("z").count("3") == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("x").count("2") == 0);
    }

    SECTION("Get all variables inside whiles") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_while_var("1", "x");
        write_facade->add_while_var("1", "y");
        write_facade->add_while_var("2", "x");
        write_facade->add_while_var("2", "z");

        REQUIRE(read_facade->get_vars_in_any_while().size() == 3);

        REQUIRE(read_facade->get_vars_in_any_while().count("x") == 1);
        REQUIRE(read_facade->get_vars_in_any_while().count("y") == 1);
        REQUIRE(read_facade->get_vars_in_any_while().count("z") == 1);
        REQUIRE(read_facade->get_vars_in_any_while().count("a") == 0);
    }

    SECTION("Get all variables inside specific while") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_while_var("1", "x");
        write_facade->add_while_var("1", "y");
        write_facade->add_while_var("2", "x");
        write_facade->add_while_var("2", "z");

        REQUIRE(read_facade->get_vars_in_while("1").size() == 2);
        REQUIRE(read_facade->get_vars_in_while("2").size() == 2);
        REQUIRE(read_facade->get_vars_in_while("3").empty());

        REQUIRE(read_facade->get_vars_in_while("1").count("x") == 1);
        REQUIRE(read_facade->get_vars_in_while("1").count("y") == 1);
        REQUIRE(read_facade->get_vars_in_while("1").count("z") == 0);
        REQUIRE(read_facade->get_vars_in_while("2").count("x") == 1);
        REQUIRE(read_facade->get_vars_in_while("2").count("y") == 0);
        REQUIRE(read_facade->get_vars_in_while("2").count("z") == 1);
    }

    SECTION("Get all while statement and variable pairs") {
        auto [read_facade, write_facade] = PkbManager::create_facades();

        write_facade->add_while_var("1", "x");
        write_facade->add_while_var("1", "y");
        write_facade->add_while_var("2", "x");
        write_facade->add_while_var("2", "z");

        REQUIRE(read_facade->get_all_while_stmt_var_pairs().size() == 4);
    }
}