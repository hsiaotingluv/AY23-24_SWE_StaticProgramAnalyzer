#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb_manager.h"
#include "sp/main.hpp"

/**
 * Next(s1, s2) holds for any type of statements.
 * Statements are 'read', 'print', 'assign', 'call', 'while', 'if'
 * Fun fact: In a while loop, Next(s1, s1) is valid, where s1 is any statement within the while loop
 */

TEST_CASE("Test Next - All Statement Types") {
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

    SECTION("Test Next with While and If statements - success") {
        // Next Line
        REQUIRE(read_facade->has_next_relation("1", "2"));
        REQUIRE(read_facade->has_next_relation("4", "5"));
        // While Statement
        REQUIRE(read_facade->has_next_relation("18", "14")); // Loopback
        REQUIRE(read_facade->has_next_relation("14", "15")); // Loop block
        REQUIRE(read_facade->has_next_relation("14", "19")); // Next Line
        // If Statement
        REQUIRE(read_facade->has_next_relation("19", "20")); // Then block
        REQUIRE(read_facade->has_next_relation("20", "23"));
        REQUIRE(read_facade->has_next_relation("19", "21")); // Else block
        REQUIRE(read_facade->has_next_relation("22", "23"));
    }

    SECTION("Test Next with While and If statements - failure") {
        // Valid for Next* but not Next
        REQUIRE_FALSE(read_facade->has_next_relation("1", "3"));
        // Next is not generally commutative
        REQUIRE_FALSE(read_facade->has_next_relation("2", "1"));
        // Different Procedures
        REQUIRE_FALSE(read_facade->has_next_relation("3", "4"));

        // For While Statement
        REQUIRE_FALSE(read_facade->has_next_relation("19", "14")); // No Loopback
        // For If Statement
        REQUIRE_FALSE(read_facade->has_next_relation("23", "19")); // No Loopback
    }

    SECTION("Test Next implementation for If Statements - success") {
        // Make sure there is no error in the If statement traverser during link_and_move()
        REQUIRE_FALSE(read_facade->has_next_relation("20", "19"));
        REQUIRE_FALSE(read_facade->has_next_relation("20", "21"));
        REQUIRE_FALSE(read_facade->has_next_relation("21", "19"));
        REQUIRE_FALSE(read_facade->has_next_relation("22", "19"));
        REQUIRE_FALSE(read_facade->has_next_relation("22", "20"));
    }
}

TEST_CASE("Test Next for If Statements - Many Dummy Nodes") {
    std::string input = R"(
        procedure computeCentroid {
            while ((x != 0) && (y != 0)) {
                count = count + 1;
                cenX = cenX + x;
                cenY = cenY + y;
            }
            if (count == 0) then {
                flag = 1;
            } else {
                cenX = cenX / count;
                cenY = cenY / count;
            }
        }
        procedure nesting {
            if (i>j) then {
                read i;
            } else {
                if (i==j) then {
                    if (j < 0) then {
                        print i;
                    } else {
                        print j;}
                } else {
                        read j;}}
        })";

    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test Next with Dummy Nodes - success") {
        // While Statement
        REQUIRE(read_facade->has_next_relation("1", "5"));
        REQUIRE(read_facade->has_next_relation("4", "1"));
        // If Statement
        REQUIRE(read_facade->has_next_relation("5", "6"));
        REQUIRE(read_facade->has_next_relation("5", "7"));
        // Nested If Statement
        REQUIRE(read_facade->has_next_relation("11", "12"));
        REQUIRE(read_facade->has_next_relation("11", "15"));
        REQUIRE(read_facade->has_next_relation("12", "13"));
        REQUIRE(read_facade->has_next_relation("12", "14"));
    }
}

TEST_CASE("Test Next - Website Example") {
    std::string input = R"(
      procedure First {
      read x;
      read z;
      call Second; }

      procedure Second {
        x = 0;
        i = 5;
        while (i!=0) {
            x = x + 2*y;
            call Third;
            i = i - 1; }
        if (x==1) then {
            x = x+1; }
          else {
            z = 1; }
        z = z + x + i;
        y = z + 2;
        x = x * y + z; }

      procedure Third {
          z = 5;
          v = z;
          print v; })";

    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test SP and PKB Next Website - success") {
        // Taken from
        // https://nus-cs3203.github.io/course-website/contents/advanced-spa-requirements/design-abstractions.html
        // computeCentroid starts with stmt 10
        REQUIRE(read_facade->has_next_relation("4", "5"));
        REQUIRE(read_facade->has_next_relation("5", "6"));
        REQUIRE(read_facade->has_next_relation("6", "7"));
        REQUIRE(read_facade->has_next_relation("6", "10"));
        REQUIRE(read_facade->has_next_relation("8", "9"));
        REQUIRE(read_facade->has_next_relation("9", "6"));
        REQUIRE(read_facade->has_next_relation("10", "11"));
        REQUIRE(read_facade->has_next_relation("10", "12"));
        REQUIRE(read_facade->has_next_relation("11", "13"));
        REQUIRE(read_facade->has_next_relation("12", "13"));

        REQUIRE_FALSE(read_facade->has_next_relation("9", "5"));
        REQUIRE_FALSE(read_facade->has_next_relation("9", "10"));
        REQUIRE_FALSE(read_facade->has_next_relation("12", "14"));
        REQUIRE_FALSE(read_facade->has_next_relation("12", "15"));
    }
}
