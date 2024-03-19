#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb_manager.h"
#include "sp/main.hpp"

/**
 * Modifies (stmt, v) holds for all statements, except 'print'
 * i.e. Modifies(stmt, v) works on 'assign', 'read', 'while', 'if', 'call'
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

    SECTION("Test Modifies for every Statement Types - success") {
        // Assign Statement
        REQUIRE(read_facade->contains_statement_modify_var("10", "count"));
        // Read Statement
        REQUIRE(read_facade->contains_statement_modify_var("4", "x"));
        // While Statement
        REQUIRE(read_facade->contains_statement_modify_var("14", "count")); // statement s in container
        REQUIRE(read_facade->contains_statement_modify_var(
            "14", "x")); // procedure call c in the container, so Modifies(s, v) must hold
        // If Statement
        REQUIRE(read_facade->contains_statement_modify_var("19", "flag")); // statement s in then stmtlist
        REQUIRE(read_facade->contains_statement_modify_var("19", "cenX")); // statement s in else stmtlist
        // Call Statement
        REQUIRE(read_facade->contains_statement_modify_var("2", "flag")); // direct
        REQUIRE(read_facade->contains_statement_modify_var("2", "cenX")); // direct
        REQUIRE(read_facade->contains_statement_modify_var("2", "x"));    // indirect
        REQUIRE(read_facade->contains_statement_modify_var("2", "y"));    // indirect
        // Procedure
        REQUIRE(read_facade->contains_procedure_modify_var("main", "flag"));              // indirect
        REQUIRE(read_facade->contains_procedure_modify_var("computeCentroid", "normSq")); // direct
        REQUIRE(read_facade->contains_procedure_modify_var("computeCentroid", "count"));  // direct
    }

    SECTION("Test Modifies across edge cases - failure") {
        // Uses Relationship but not Modifies Relationship - failure.
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var("main", "computeCentroid"));

        // Modifies relationship across disjoint procedures - failure.
        REQUIRE_FALSE(read_facade->contains_statement_modify_var("8", "flag"));
    }
}

TEST_CASE("Test Modifies - Complex Call Relationship") {
    std::string input = R"(
        procedure main {
            x = 0;
            call proc1;
        }

        procedure proc1 {
            y = 0;
            call proc2;
            call proc3;
        }

        procedure proc2 {
            z = 0;
            call proc3;
            call proc4;
        }

        procedure proc3 {
            a = 0;
            call proc5;
        }

        procedure proc4 {
            b = 0;
        }

        procedure proc5 {
            c = 0;
        }
        
        procedure proc6 {
            d = 0;
        }
        )";

    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test Modifies across Complex Call Relationship - success") {
        REQUIRE(read_facade->contains_procedure_modify_var("main", "c"));
        REQUIRE(read_facade->contains_statement_modify_var("2", "b"));
        REQUIRE(read_facade->contains_statement_modify_var("2", "c"));
    }

    SECTION("Test Modifies across Complex Call Relationship - failure") {
        // Uses Relationship but not Modifies Relationship - failure.
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var("proc4", "c"));
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var(
            "proc2", "y")); // Should not be able to access caller's variables.
        REQUIRE_FALSE(read_facade->contains_procedure_modify_var("main", "d")); // Disjoint Call Graph
    }
}