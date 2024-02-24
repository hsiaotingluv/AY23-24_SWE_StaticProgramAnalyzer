#include <catch.hpp>

#include "pkb/stores/assignment_store.h"

TEST_CASE("Assignment Store Tests") {
    AssignmentStore assignment_store;

    SECTION("Simple assignment store test") {
        auto vx = Variable("x");
        auto vy = Variable("y");
        auto vz = Variable("z");
        auto vq = Variable("q");
        auto vp = Variable("p");

        assignment_store.add_assignment("1", vx, "1");
        assignment_store.add_assignment("2", vy, "x");
        assignment_store.add_assignment("3", vz, "2");
        assignment_store.add_assignment("4", vq, "y");
        assignment_store.add_assignment("5", vx, "1");

        REQUIRE(assignment_store.get_all_assignments_rhs("1").size() == 2);
        REQUIRE(assignment_store.get_all_assignments_rhs("2").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_rhs("x").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_rhs("y").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_rhs("z").size() == 0);

        REQUIRE(assignment_store.get_all_assignments_lhs(vx).size() == 2);
        REQUIRE(assignment_store.get_all_assignments_lhs(vy).size() == 1);
        REQUIRE(assignment_store.get_all_assignments_lhs(vz).size() == 1);
        REQUIRE(assignment_store.get_all_assignments_lhs(vq).size() == 1);
        REQUIRE(assignment_store.get_all_assignments_lhs(vp).size() == 0);

        REQUIRE(assignment_store.get_all_assignments_lhs_rhs(vx, "1").size() == 2);
        REQUIRE(assignment_store.get_all_assignments_lhs_rhs(vy, "x").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_lhs_rhs(vz, "2").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_lhs_rhs(vq, "y").size() == 1);
    }

    SECTION("More complex assignment store test") {
        auto vx = Variable("x");
        auto vy = Variable("y");
        auto vz = Variable("z");
        auto vq = Variable("q");
        auto vp = Variable("p");
        auto vxy = Variable("xy");

        assignment_store.add_assignment("1", vx, "1");
        assignment_store.add_assignment("2", vy, "x");
        assignment_store.add_assignment("3", vz, "2");
        assignment_store.add_assignment("4", vq, "y");
        assignment_store.add_assignment("5", vx, "1");
        assignment_store.add_assignment("6", vx, "2");
        assignment_store.add_assignment("7", vxy, "3 4 +");
        assignment_store.add_assignment("8", vx, "1 y +");
        assignment_store.add_assignment("9", vxy, "y 1 +");

        // Test partial RHS match, any lhs
        REQUIRE(assignment_store.get_all_assignments_rhs_partial("1").size() == 4);
        REQUIRE(assignment_store.get_all_assignments_rhs_partial("2").size() == 2);
        REQUIRE(assignment_store.get_all_assignments_rhs_partial("x").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_rhs_partial("y").size() == 3);
        REQUIRE(assignment_store.get_all_assignments_rhs_partial("z").size() == 0);

        // Test partial RHS match, exact lhs
        REQUIRE(assignment_store.get_all_assignments_lhs_rhs_partial(vx, "1").size() == 3);
        REQUIRE(assignment_store.get_all_assignments_lhs_rhs_partial(vy, "x").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_lhs_rhs_partial(vz, "2").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_lhs_rhs_partial(vq, "y").size() == 1);
        REQUIRE(assignment_store.get_all_assignments_lhs_rhs_partial(vx, "1").size() == 3);
    }
}