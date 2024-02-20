#include <catch.hpp>
#include <memory>

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"

TEST_CASE("Simple Entity Test") {
    SECTION("Procedure Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_procedure("procedure1");

        REQUIRE(read_facade->get_procedures().size() == 1);
        REQUIRE(read_facade->get_variables().empty());
        REQUIRE(read_facade->get_entities().size() == 1);
    }
}

TEST_CASE("Simple Statement Test") {
    SECTION("Statement Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Print);
        write_facade->add_statement("3", StatementType::Assign);
        write_facade->add_statement("4", StatementType::If);
        write_facade->add_statement("5", StatementType::While);
        write_facade->add_statement("6", StatementType::Call);
        write_facade->add_statement("7", StatementType::Raw);

        REQUIRE(read_facade->get_all_statements().size() == 7);
        REQUIRE(read_facade->get_read_statements().size() == 1);
        REQUIRE(read_facade->get_assign_statements().size() == 1);
        REQUIRE(read_facade->get_print_statements().size() == 1);
        REQUIRE(read_facade->get_if_statements().size() == 1);
        REQUIRE(read_facade->get_while_statements().size() == 1);
        REQUIRE(read_facade->get_call_statements().size() == 1);
        REQUIRE(read_facade->get_raw_statements().size() == 1);
    }
}

TEST_CASE("Follows and FollowsStar Relationship Test") {
    SECTION("Follows and FollowsStar Transitive Relationship Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->add_follows("4", "5");
        write_facade->finalise_pkb();

        REQUIRE(read_facade->has_follows("1", "2"));
        REQUIRE(read_facade->has_follows("2", "3"));
        REQUIRE(read_facade->has_follows("3", "4"));
        REQUIRE(read_facade->has_follows("4", "5"));

        REQUIRE(read_facade->has_follows_stars("1", "2"));
        REQUIRE(read_facade->has_follows_stars("1", "3"));
        REQUIRE(read_facade->has_follows_stars("1", "4"));
        REQUIRE(read_facade->has_follows_stars("1", "5"));
        REQUIRE(read_facade->has_follows_stars("2", "3"));
        REQUIRE(read_facade->has_follows_stars("2", "4"));
        REQUIRE(read_facade->has_follows_stars("2", "5"));
        REQUIRE(read_facade->has_follows_stars("3", "4"));
        REQUIRE(read_facade->has_follows_stars("3", "5"));
        REQUIRE(read_facade->has_follows_stars("4", "5"));
    }

    SECTION("Get All Followee By Statement Type Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

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
        auto [read_facade, write_facade] = PKB::create_facades();

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
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Print);
        write_facade->add_statement("3", StatementType::While);
        write_facade->add_statement("4", StatementType::While);

        REQUIRE(read_facade->get_follows_following("1") == "2");
        REQUIRE(read_facade->get_follows_stars_following("1").size() == 3);
        REQUIRE(read_facade->get_follows_following("1", StatementType::Read).empty());
        REQUIRE(read_facade->get_follows_following("1", StatementType::Print) == "2");
        REQUIRE(read_facade->get_follows_stars_following("1", StatementType::Read).empty());
        REQUIRE(read_facade->get_follows_stars_following("1", StatementType::While).size() == 2);
    }

    SECTION("Get All Followee by Follower Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_follows("1", "2");
        write_facade->add_follows("2", "3");
        write_facade->add_follows("3", "4");
        write_facade->finalise_pkb();

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::While);
        write_facade->add_statement("3", StatementType::While);
        write_facade->add_statement("4", StatementType::While);

        REQUIRE(read_facade->get_follows_by("2") == "1");
        REQUIRE(read_facade->get_follows_stars_by("4").size() == 3);
        REQUIRE(read_facade->get_follows_by("2", StatementType::Read) == "1");
        REQUIRE(read_facade->get_follows_by("2", StatementType::Print).empty());
        REQUIRE(read_facade->get_follows_stars_by("4", StatementType::While).size() == 2);
        REQUIRE(read_facade->get_follows_stars_by("4", StatementType::If).empty());
    }
}

TEST_CASE("Parent and ParentStar Relationship Test") {
    SECTION("Parent and ParentStar Transitive Relationship Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_parent("1", "2");
        write_facade->add_parent("1", "3");
        write_facade->add_parent("3", "4");
        write_facade->add_parent("3", "5");
        write_facade->add_parent("3", "6");
        write_facade->add_parent("6", "7");
        write_facade->finalise_pkb();

        REQUIRE(read_facade->has_parent("1", "2"));
        REQUIRE(read_facade->has_parent("1", "3"));
        REQUIRE(read_facade->has_parent("3", "4"));
        REQUIRE(read_facade->has_parent("3", "5"));
        REQUIRE(read_facade->has_parent("3", "6"));
        REQUIRE(read_facade->has_parent("6", "7"));

        REQUIRE(read_facade->has_parent_star("1", "2"));
        REQUIRE(read_facade->has_parent_star("1", "3"));
        REQUIRE(read_facade->has_parent_star("1", "4"));
        REQUIRE(read_facade->has_parent_star("1", "5"));
        REQUIRE(read_facade->has_parent_star("1", "6"));
        REQUIRE(read_facade->has_parent_star("1", "7"));
    }

    SECTION("Get All Parents By Statement Type Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

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
        auto [read_facade, write_facade] = PKB::create_facades();

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
        auto [read_facade, write_facade] = PKB::create_facades();

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

        REQUIRE(read_facade->get_parent_children("1").size() == 1);
        REQUIRE(read_facade->get_parent_star_children("1").size() == 4);
        REQUIRE(read_facade->get_parent_children("1", StatementType::While).size() == 1);
        REQUIRE(read_facade->get_parent_children("1", StatementType::If).empty());
        REQUIRE(read_facade->get_parent_star_children("1", StatementType::While).size() == 2);
        REQUIRE(read_facade->get_parent_star_children("1", StatementType::If).size() == 2);
    }

    SECTION("Get All Parent by Children Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

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

        REQUIRE(read_facade->get_parent("2") == "1");
        REQUIRE(read_facade->get_star_parent("4").size() == 3);
        REQUIRE(read_facade->get_parent("2", StatementType::If) == "1");
        REQUIRE(read_facade->get_parent("2", StatementType::While).empty());
        REQUIRE(read_facade->get_star_parent("5", StatementType::If).size() == 3);
        REQUIRE(read_facade->get_star_parent("5", StatementType::While).size() == 1);
    }
}

TEST_CASE("Modify Test") {
    SECTION("Simple Modify Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_statement_modifies_var("1", "x");
        write_facade->add_statement_modifies_var("1", "y");
        write_facade->add_statement_modifies_var("2", "x");
        write_facade->add_statement_modifies_var("2", "z");

        REQUIRE(read_facade->does_statement_modify_var("1", "x"));
        REQUIRE(!read_facade->does_statement_modify_var("1", "z"));
        REQUIRE(read_facade->get_statements_that_modify_var("x").size() == 2);
        REQUIRE(read_facade->get_statements_that_modify_var("z").size() == 1);
        REQUIRE(read_facade->get_vars_modified_by_statement("1").size() == 2);
    }

    SECTION("More complex modify test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_procedure_modifies_var("main", "x");
        write_facade->add_procedure_modifies_var("main", "y");
        write_facade->add_procedure_modifies_var("helper", "z");
        write_facade->add_statement_modifies_var("1", "x");
        write_facade->add_statement_modifies_var("1", "y");
        write_facade->add_statement_modifies_var("2", "x");
        write_facade->add_statement_modifies_var("2", "z");

        REQUIRE(read_facade->get_all_statements_that_modify().size() == 2);
        REQUIRE(read_facade->does_statement_modify_any_var("1"));
        REQUIRE(read_facade->get_all_statements_and_var_modify_pairs().size() == 4);
        REQUIRE(read_facade->get_all_procedures_that_modify().size() == 2);
        REQUIRE(read_facade->does_procedure_modify_any_var("main"));
        REQUIRE(read_facade->get_all_procedures_and_var_modify_pairs().size() == 3);

        // Negative testcases
        REQUIRE_FALSE(read_facade->does_statement_modify_any_var("3"));
        REQUIRE_FALSE(read_facade->does_procedure_modify_any_var("nonexistent"));
    }

    SECTION("Test get_statements_that_modify_var of certain type") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_statement_modifies_var("1", "x");
        write_facade->add_statement_modifies_var("1", "y");
        write_facade->add_statement_modifies_var("2", "x");
        write_facade->add_statement_modifies_var("2", "z");

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Call);

        REQUIRE(read_facade->get_statements_that_modify_var("x", StatementType::Read).size() == 1);
        REQUIRE(read_facade->get_statements_that_modify_var("z", StatementType::Call).size() == 1);
        REQUIRE(read_facade->get_statements_that_modify_var("x", StatementType::Print).size() == 0);
        REQUIRE(read_facade->get_statements_that_modify_var("a", StatementType::If).size() == 0);
        REQUIRE(read_facade->get_statements_that_modify_var("b", StatementType::Read).size() == 0);
    }
}

TEST_CASE("Use Test") {
    SECTION("Simple Use Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_statement_uses_var("1", "x");
        write_facade->add_statement_uses_var("1", "y");
        write_facade->add_statement_uses_var("2", "x");
        write_facade->add_statement_uses_var("2", "z");

        REQUIRE(read_facade->does_statement_use_var("1", "x"));
        REQUIRE(!read_facade->does_statement_use_var("1", "z"));
        REQUIRE(read_facade->get_statements_that_use_var("x").size() == 2);
        REQUIRE(read_facade->get_statements_that_use_var("z").size() == 1);
        REQUIRE(read_facade->get_vars_used_by_statement("1").size() == 2);
    }

    SECTION("More complex use test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_procedure_uses_var("main", "x");
        write_facade->add_procedure_uses_var("main", "y");
        write_facade->add_procedure_uses_var("helper", "z");
        write_facade->add_statement_uses_var("1", "x");
        write_facade->add_statement_uses_var("1", "y");
        write_facade->add_statement_uses_var("2", "x");
        write_facade->add_statement_uses_var("2", "z");

        REQUIRE(read_facade->get_all_statements_that_use().size() == 2);
        REQUIRE(read_facade->does_statement_use_any_var("1"));
        REQUIRE(read_facade->get_all_statements_and_var_use_pairs().size() == 4);
        REQUIRE(read_facade->get_all_procedures_that_use().size() == 2);
        REQUIRE(read_facade->does_procedure_use_any_var("main"));
        REQUIRE(read_facade->get_all_procedures_and_var_use_pairs().size() == 3);

        // Negative testcases
        REQUIRE_FALSE(read_facade->does_statement_use_any_var("3"));
        REQUIRE_FALSE(read_facade->does_procedure_use_any_var("nonexistent"));
    }

    SECTION("Test get_statements_that_use_var of certain type") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_statement_uses_var("1", "x");
        write_facade->add_statement_uses_var("1", "y");
        write_facade->add_statement_uses_var("2", "x");
        write_facade->add_statement_uses_var("2", "z");

        write_facade->add_statement("1", StatementType::Read);
        write_facade->add_statement("2", StatementType::Call);

        REQUIRE(read_facade->get_statements_that_use_var("x", StatementType::Read).size() == 1);
        REQUIRE(read_facade->get_statements_that_use_var("z", StatementType::Call).size() == 1);
        REQUIRE(read_facade->get_statements_that_use_var("x", StatementType::Print).size() == 0);
        REQUIRE(read_facade->get_statements_that_use_var("a", StatementType::If).size() == 0);
        REQUIRE(read_facade->get_statements_that_use_var("b", StatementType::Read).size() == 0);
    }
}

TEST_CASE("Assignment Pattern Test") {
    SECTION("Simple Assignment Pattern Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_assignment("1", "x", "1");
        write_facade->add_assignment("2", "y", "x");
        write_facade->add_assignment("3", "z", "2");
        write_facade->add_assignment("4", "q", "y");
        write_facade->add_assignment("5", "x", "1");

        // Test exact rhs
        REQUIRE(read_facade->get_all_assignments_rhs("1").size() == 2);
        REQUIRE(read_facade->get_all_assignments_rhs("2").size() == 1);
        REQUIRE(read_facade->get_all_assignments_rhs("x").size() == 1);
        REQUIRE(read_facade->get_all_assignments_rhs("y").size() == 1);
        REQUIRE(read_facade->get_all_assignments_rhs("z").size() == 0);

        // Test exact lhs
        REQUIRE(read_facade->get_all_assignments_lhs("x").size() == 2);
        REQUIRE(read_facade->get_all_assignments_lhs("y").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs("z").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs("q").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs("p").size() == 0);

        // Test exact lhs and exact rhs
        REQUIRE(read_facade->get_all_assignments_lhs_rhs("x", "1").size() == 2);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs("y", "x").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs("z", "2").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs("q", "y").size() == 1);
    }

    SECTION("more complex assignment pattern test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_assignment("1", "x", "1");
        write_facade->add_assignment("2", "y", "x");
        write_facade->add_assignment("3", "z", "2");
        write_facade->add_assignment("4", "q", "y");
        write_facade->add_assignment("5", "x", "1");
        write_facade->add_assignment("6", "x", "2");
        write_facade->add_assignment("7", "xy", "3 4 +");
        write_facade->add_assignment("8", "x", "1 y +");
        write_facade->add_assignment("9", "xy", "y 1 +");

        // Test partial RHS match, any lhs
        REQUIRE(read_facade->get_all_assignments_rhs_partial("1").size() == 4);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("2").size() == 2);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("x").size() == 1);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("y").size() == 3);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("z").size() == 0);

        // Test partial RHS match, exact lhs
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("x", "1").size() == 3);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("y", "x").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("z", "2").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("q", "y").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("x", "1").size() == 3);
    }
}