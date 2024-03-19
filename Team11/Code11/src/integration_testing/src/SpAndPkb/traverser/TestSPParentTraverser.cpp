#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb.h"
#include "sp/main.hpp"

/**
 * Parent(s1, s2) holds for any type of statements.
 * Statements are 'read', 'print', 'call', 'while', 'if'
 * Fun fact: If s1 is 'read', 'print', 'call', then Parent(s1,s2) must be False.
 */

TEST_CASE("Test Parent - All Statement Types") {
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

    SECTION("Test Parent from Website - success") {
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

    SECTION("Test Parent across While and If statements - success") {
        // While statement
        REQUIRE(read_facade->has_parent_relation("14", "15"));
        REQUIRE(read_facade->has_parent_relation("14", "18"));
        REQUIRE(read_facade->has_parent_star_relation("14", "18"));
        // If statement
        REQUIRE(read_facade->has_parent_relation("19", "20"));
        REQUIRE(read_facade->has_parent_relation("19", "21"));
        REQUIRE(read_facade->has_parent_relation("19", "22"));
        REQUIRE(read_facade->has_parent_star_relation("19", "22"));
    }

    SECTION("Test Parent across edge cases - failure") {
        // Parent relationship across procedures - failure.
        REQUIRE_FALSE(read_facade->has_parent_relation("4", "10"));
        REQUIRE_FALSE(read_facade->has_parent_relation("4", "15"));
        // Parent relationship between statements for one nesting level, but not directly nested - failure.
        REQUIRE_FALSE(read_facade->has_parent_relation("10", "15"));
        REQUIRE_FALSE(read_facade->has_parent_relation("14", "20"));
    }
}

TEST_CASE("Test Parent - Advanced SPA with nested While & If loops") {
    std::string input = R"(
        procedure First {
            while (x==1) {
                while (x==2) {
                    while (x==3) {
                        read me;
                    }
                }
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
        })";

    auto [read_facade, write_facade] = pkb::PKB::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test Parent across While and If statements - success") {
        // While statement
        REQUIRE(read_facade->has_parent_relation("1", "2"));
        REQUIRE(read_facade->has_parent_relation("2", "3"));
        REQUIRE(read_facade->has_parent_relation("3", "4"));
        REQUIRE(read_facade->has_parent_star_relation("1", "4"));
        // If statement
        REQUIRE(read_facade->has_parent_relation("5", "6"));
        REQUIRE(read_facade->has_parent_relation("6", "7"));
        REQUIRE(read_facade->has_parent_relation("9", "11"));
        REQUIRE(read_facade->has_parent_star_relation("5", "7"));
        REQUIRE(read_facade->has_parent_star_relation("5", "11"));
    }
}