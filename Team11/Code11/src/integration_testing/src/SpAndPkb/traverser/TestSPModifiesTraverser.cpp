#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb_manager.h"
#include "sp/main.hpp"

/**
 * Modifies (s, v) holds for all statements, except 'print'
 * i.e. Modifies(s, v) works on 'assign', 'read', 'while', 'if', 'call'
 * Modifies (p, v) holds for procedure. This requires a deep search of nested statements for variables.
 */

TEST_CASE("Test Modifies - All Statement Types") {
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

    SECTION("Test SP and PKB Modifies Website - success") {
        // Taken from
        // https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/design-abstractions.html#code-5
        // computeCentroid starts with stmt 10
        REQUIRE(read_facade->contains_statement_modify_var("10", "count"));
        REQUIRE(read_facade->contains_statement_modify_var("16", "cenX"));
        REQUIRE(read_facade->contains_statement_modify_var("18", "x"));
        REQUIRE(read_facade->contains_statement_modify_var("19", "flag"));
        REQUIRE(read_facade->contains_statement_modify_var("14", "x"));
        REQUIRE(read_facade->contains_procedure_modify_var("main", "y"));

        REQUIRE_FALSE(read_facade->contains_statement_modify_var("14", "flag"));
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var("printResults", "normSq"));
    }

    SECTION("Test Modifies with Procedures and Calls - success") {
        // Procedures
        REQUIRE(read_facade->contains_procedure_modify_var("main", "flag"));
        REQUIRE(read_facade->contains_procedure_modify_var("computeCentroid", "normSq"));
        REQUIRE(read_facade->contains_procedure_modify_var("computeCentroid", "count"));

        // Calls Statement
        REQUIRE(read_facade->contains_statement_modify_var("2", "count"));
        REQUIRE(read_facade->contains_statement_modify_var("2", "cenX"));
        REQUIRE(read_facade->contains_statement_modify_var("2", "y"));
        REQUIRE(read_facade->contains_statement_modify_var("18", "x"));
    }

    SECTION("Test Modifies across edge cases - failure") {
        // Uses Relationship but not Modifies Relationship - failure.
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var("main", "computeCentroid"));

        // Modifies relationship across disjoint procedures - failure.
        REQUIRE_FALSE(read_facade->contains_statement_modify_var("8", "flag"));
    }
}

TEST_CASE("Test Modifies - Advanced SPA with nested While & If loops") {
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

    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);
}