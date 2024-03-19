#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb.h"
#include "sp/main.hpp"

/**
 * Follows (s1, s2) holds for any type of statements.
 * Statements are 'read', 'print', 'call', 'while', 'if'
 * One Point: Follows(s1, s2) != Follows(s2, s1).
 */

TEST_CASE("Test Follows - All Statement Types") {
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

    auto [read_facade, write_facade] = pkb::PKB::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test Follows from Website - success") {
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

    SECTION("Test Follows across While and If statements - success") {
        // While statement
        REQUIRE(read_facade->has_follows_relation("14", "19"));
        REQUIRE_FALSE(read_facade->has_follows_relation("14", "15"));
        REQUIRE_FALSE(read_facade->has_follows_relation("14", "18"));

        // If statement
        REQUIRE(read_facade->has_follows_relation("19", "23"));
        REQUIRE_FALSE(read_facade->has_follows_relation("19", "20"));
        REQUIRE_FALSE(read_facade->has_follows_relation("19", "21"));
        REQUIRE_FALSE(read_facade->has_follows_relation("19", "22"));
    }

    SECTION("Test Follows across edge cases - failure") {
        // Follows relationship across procedures - failure.
        REQUIRE_FALSE(read_facade->has_follows_relation("4", "6"));
        REQUIRE_FALSE(read_facade->has_follows_relation("6", "10"));
        // Follows relationship between statements not immediately after each other - failure.
        REQUIRE_FALSE(read_facade->has_follows_relation("10", "12")); // Follows(10, 12) is false.
        REQUIRE(read_facade->has_follows_star_relation("10", "12"));  // Follows*(10, 12) is true.
        REQUIRE_FALSE(read_facade->has_follows_relation("14", "23")); // Follows*(15, 28) is false.
        REQUIRE(read_facade->has_follows_star_relation("14", "23"));  // Follows*(15, 28) is true.
    }
}

TEST_CASE("Test Follows - Advanced SPA with nested While & If loops") {
    std::string input = R"(
        procedure First {
            while (x==1) {
                while (x==2) {
                    while (x==3) {
                        read me;
                    }
                    read her;
                }
                read him;
            }
            if (x==1)
            then { 
                if (x==2)
                then {read me;}
                else {read me;}}
            else {
                if (x==(1/0))
                then {read me;}
                else {read me;}}
            print fake;
        })";

    auto [read_facade, write_facade] = pkb::PKB::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test Follows across While and If statements - success") {
        // While statement
        REQUIRE(read_facade->has_follows_relation("1", "7"));
        REQUIRE(read_facade->has_follows_relation("2", "6"));
        REQUIRE(read_facade->has_follows_relation("3", "5"));
        REQUIRE(read_facade->has_follows_star_relation("1", "14"));

        REQUIRE_FALSE(read_facade->has_follows_relation("1", "2"));  // Diff Nesting lv;
        REQUIRE_FALSE(read_facade->has_follows_relation("1", "14")); // Not immediately after
        // If statement
        REQUIRE(read_facade->has_follows_relation("7", "14"));

        REQUIRE_FALSE(read_facade->has_follows_relation("7", "8"));  // Diff Nesting lv;
        REQUIRE_FALSE(read_facade->has_follows_relation("8", "11")); // Diff Stmt List;
        REQUIRE_FALSE(read_facade->has_follows_relation("9", "10")); // Diff Stmt List;
    }
}