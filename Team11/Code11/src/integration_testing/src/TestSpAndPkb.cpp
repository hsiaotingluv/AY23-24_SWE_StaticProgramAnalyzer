#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb_manager.h"
#include "sp/main.hpp"

TEST_CASE("Test SP and PKB - Basic SPA") {
    std::string input = R"(
        procedure main {
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

    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test SP and PKB Assignment Pattern - success") {
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
        REQUIRE(read_facade->get_if_stmts_with_var().size() == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("count").size() == 1);
        REQUIRE(read_facade->get_if_stmts_with_var("flag").empty());

        REQUIRE(read_facade->get_while_stmts_with_var().size() == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("x").size() == 1);
        REQUIRE(read_facade->get_while_stmts_with_var("y").size() == 1);
    }

    SECTION("Test SP and PKB Populate calls - success") {
        REQUIRE(read_facade->get_all_calls_callers().size() == 2);
        REQUIRE(read_facade->get_all_calls_callees().size() == 3);
        REQUIRE(read_facade->get_callees("main").size() == 2);
        REQUIRE(read_facade->get_callers("main").empty());
        REQUIRE(read_facade->get_callers("computeCentroid").size() == 1);
    }
}