#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb.h"
#include "sp/main.hpp"

TEST_CASE("Test SP and PKB") {
    auto [read_facade, write_facade] = PKB::create_facades();

    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);

    std::string input = R"(procedure main {
            flag = 0;
            call computeCentroid;
            call printResults;
        }

        procedure readPoint {
            read x;
            read y;
        }

        procedure printResults {
            print flag;
            print cenX;
            print cenY;
            print normSq;
        }

        procedure computeCentroid {
            count = 0;
            cenX = 0;
            cenY = 0;
            call readPoint;
            while ((x != 0) && (y != 0)) {
                count = count + 1;
                cenX = cenX + x;
                cenY = cenY + y;
                call readPoint;
            }
            if (count == 0) then {
                flag = 1;
            } else {
                cenX = cenX / count;
                cenY = cenY / count;
            }
            normSq = cenX * cenX + cenY * cenY;
        })";

    SECTION("Test SP and PKB Uses Website - success") {
        auto ast = sp->process(input);

        // Taken from
        // https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/design-abstractions.html#uses
        // computeCentroid starts with stmt 10
        REQUIRE(read_facade->does_statement_use_var("16", "x"));
        REQUIRE(read_facade->does_statement_use_var("19", "count"));
        REQUIRE(read_facade->does_statement_use_var("19", "cenX"));
        REQUIRE(read_facade->does_procedure_use_var("main", "cenX"));
        REQUIRE(read_facade->does_procedure_use_var("main", "flag"));
        REQUIRE(read_facade->does_procedure_use_var("computeCentroid", "x"));

        REQUIRE_FALSE(read_facade->does_statement_use_var("12", "count"));
        REQUIRE_FALSE(read_facade->does_statement_use_var("19", "flag"));
        REQUIRE_FALSE(read_facade->does_statement_use_var("18", "y"));
    }

    SECTION("Test SP and PKB Parent Website - success") {
        auto ast = sp->process(input);

        // Taken from
        // https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/design-abstractions.html#code-5
        // computeCentroid starts with stmt 10
        REQUIRE(read_facade->has_parent_relation("14", "16"));
        REQUIRE(read_facade->has_parent_relation("14", "17"));
        REQUIRE(read_facade->has_parent_relation("19", "20"));
        REQUIRE(read_facade->has_parent_star_relation("14", "16"));
        REQUIRE(read_facade->has_parent_star_relation("19", "22"));

        REQUIRE_FALSE(read_facade->has_parent_relation("10", "11"));
        REQUIRE_FALSE(read_facade->has_parent_relation("10", "15"));
        REQUIRE_FALSE(read_facade->has_parent_relation("14", "19"));
    }

    SECTION("Test SP and PKB Modifies Website - success") {
        auto ast = sp->process(input);

        // Taken from
        // https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/design-abstractions.html#code-5
        // computeCentroid starts with stmt 10
        REQUIRE(read_facade->does_statement_modify_var("10", "count"));
        REQUIRE(read_facade->does_statement_modify_var("16", "cenX"));
        REQUIRE(read_facade->does_statement_modify_var("18", "x"));
        REQUIRE(read_facade->does_statement_modify_var("19", "flag"));
        REQUIRE(read_facade->does_statement_modify_var("14", "x"));
        REQUIRE(read_facade->does_procedure_modify_var("main", "y"));

        REQUIRE_FALSE(read_facade->does_statement_modify_var("14", "flag"));
        REQUIRE_FALSE(read_facade->does_procedure_modify_var("printResults", "normSq"));
    }

    SECTION("Test SP and PKB Follows Website - success") {
        auto ast = sp->process(input);

        // Taken from
        // https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/design-abstractions.html#follows--follows
        // computeCentroid starts with stmt 10
        REQUIRE(read_facade->has_follows_relation("10", "11"));
        REQUIRE(read_facade->has_follows_relation("13", "14"));
        REQUIRE(read_facade->has_follows_relation("14", "19"));
        REQUIRE(read_facade->has_follows_star_relation("12", "19"));
        REQUIRE(read_facade->has_follows_star_relation("10", "23"));

        REQUIRE_FALSE(read_facade->has_follows_relation("14", "15"));
        REQUIRE_FALSE(read_facade->has_follows_relation("18", "19"));
        REQUIRE_FALSE(read_facade->has_follows_relation("20", "21"));
        REQUIRE_FALSE(read_facade->has_follows_star_relation("21", "23"));
    }

    SECTION("Test SP and PKB Assignment Pattern - success") {
        auto ast = sp->process(input);

        REQUIRE(read_facade->get_all_assignments_lhs("flag").size() == 2);
        REQUIRE(read_facade->get_all_assignments_lhs("count").size() == 2);
        REQUIRE(read_facade->get_all_assignments_lhs("cenX").size() == 3);
        REQUIRE(read_facade->get_all_assignments_lhs("cenY").size() == 3);

        REQUIRE(read_facade->get_all_assignments_rhs("0 ").size() == 4);
        REQUIRE(read_facade->get_all_assignments_rhs("cenX ").empty());

        REQUIRE(read_facade->get_all_assignments_rhs_partial("cenX ").size() == 3);
        REQUIRE(read_facade->get_all_assignments_rhs_partial("cenX cenX * ").size() == 1);

        REQUIRE(read_facade->get_all_assignments_lhs_rhs("flag", "1 ").size() == 1);
        REQUIRE(read_facade->get_all_assignments_lhs_rhs_partial("cenX", "cenX ").size() == 2);
    }

    SECTION("Test SP and PKB While and If Pattern - success") {
        auto ast = sp->process(input);

        REQUIRE(read_facade->get_if_stmts_with_var().size() == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("count").size() == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("flag").empty());

        REQUIRE(read_facade->get_while_stmts_with_var().size() == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("x").size() == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("y").size() == 1);
    }

    SECTION("Test SP and PKB Populate calls - success") {
        auto ast = sp->process(input);

        REQUIRE(read_facade->get_all_calls_keys().size() == 2);
        REQUIRE(read_facade->get_all_calls_values().size() == 3);
        REQUIRE(read_facade->get_callees("main").size() == 2);
        REQUIRE(read_facade->get_callers("main").empty());
        REQUIRE(read_facade->get_callers("computeCentroid").size() == 1);
    }
}