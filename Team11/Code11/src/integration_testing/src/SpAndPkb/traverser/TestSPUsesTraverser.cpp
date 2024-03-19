#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb_manager.h"
#include "sp/main.hpp"

/**
 * Uses(stmt, v) holds for all statements, except 'read'
 * i.e. Uses(stmt, v) works on 'assign', 'print', 'while', 'if', 'call'
 * Uses(p, v) holds for procedure. This requires a deep search of nested statements for variables.
 */

TEST_CASE("Test Uses - All Statement Types") {
    std::string input = R"(
        procedure main {
            flag = 0;1
            call computeCentroid;2
            call printResults;3
        }

        procedure readPoint {
            read x;4
            read y;5
        }

        procedure printResults {
            print flag;6
            print cenX;7
            print cenY;8
            print normSq;9
        }

        procedure computeCentroid {
            count = 0;0
            cenX = 0;1
            cenY = 0;2
            call readPoint;3
            while ((x != count) && (y != cenX)) {4
                count = count + 1;5
                cenX = cenX + x;6
                cenY = cenY + y;7
                call readPoint;8
            }
            if (count == 0) then {9
                flag = 1;0
                cenX = count / count;
            } else {
                cenX = cenX / count;1
                cenY = cenY / count;2
            }
            call printResults;3
        })";

    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test SP and PKB Uses Website - success") {
        // Taken from
        // https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/design-abstractions.html#uses
        // computeCentroid starts with stmt 10
        REQUIRE(read_facade->contains_statement_use_var("16", "x"));
        REQUIRE(read_facade->contains_statement_use_var("19", "count"));
        REQUIRE(read_facade->contains_statement_use_var("19", "cenX"));
        REQUIRE(read_facade->contains_procedure_use_var("main", "cenX"));
        REQUIRE(read_facade->contains_procedure_use_var("main", "flag"));
        REQUIRE(read_facade->contains_procedure_use_var("computeCentroid", "x"));

        REQUIRE_FALSE(read_facade->contains_statement_use_var("12", "count"));
        REQUIRE_FALSE(read_facade->contains_statement_use_var("19", "flag"));
        REQUIRE_FALSE(read_facade->contains_statement_use_var("18", "y"));
    }

    SECTION("Test Modifies for every Statement Types - success") {
        // Assign Statement
        REQUIRE(read_facade->contains_statement_use_var("15", "count"));
        // Print Statement
        REQUIRE(read_facade->contains_statement_use_var("6", "flag"));
        // While Statement
        REQUIRE(read_facade->contains_statement_use_var("14", "count")); // statement s in container
        REQUIRE(read_facade->contains_statement_use_var("14", "cenX")); // procedure call c in the container, so Uses(s, v) must hold
        // If Statement
        REQUIRE(read_facade->contains_statement_use_var("19", "count")); // statement s in then stmtlist
        REQUIRE(read_facade->contains_statement_use_var("19", "cenX")); // statement s in else stmtlist
        // Call Statement
        REQUIRE(read_facade->contains_statement_use_var("2", "count")); // direct
        REQUIRE(read_facade->contains_statement_use_var("2", "cenX")); // direct
        REQUIRE(read_facade->contains_statement_use_var("2", "flag")); // indirect
        REQUIRE(read_facade->contains_statement_use_var("2", "normSq")); // indirect
        // Procedure
        REQUIRE(read_facade->contains_statement_use_var("main", "flag")); // indirect
        REQUIRE(read_facade->contains_procedure_use_var("computeCentroid", "normSq")); // indirect
        REQUIRE(read_facade->contains_procedure_use_var("computeCentroid", "count"));  // direct
    }

    SECTION("Test Modifies across edge cases - failure") {
        // Modifies Relationship but not Uses Relationship - failure.
        REQUIRE_FALSE(read_facade->contains_statement_use_var("18", "x"));

        // Modifies relationship across disjoint procedures - failure.
        REQUIRE_FALSE(read_facade->contains_statement_use_var("6", "x"));
    }
}

TEST_CASE("Test Uses - Complex Call Relationship") {
    std::string input = R"(
        procedure main {
            x = i;
            call proc1;
        }

        procedure proc1 {
            y = u;
            call proc2;
            call proc3;
        }

        procedure proc2 {
            z = t;
            call proc3;
            call proc4;
        }

        procedure proc3 {
            a = r;
            call proc5;
        }

        procedure proc4 {
            b = e;
        }

        procedure proc5 {
            c = w;
        }
        
        procedure proc6 {
            d = q;
        }
        )";

    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test Uses across Complex Call Relationship - success") {
        REQUIRE(read_facade->contains_procedure_use_var("main", "w"));
        REQUIRE(read_facade->contains_statement_use_var("2", "e"));
        REQUIRE(read_facade->contains_statement_use_var("2", "r"));
    }

    SECTION("Test Uses across Complex Call Relationship - failure") {
        // Modifies Relationship but not Uses Relationship - failure.
        REQUIRE_FALSE(read_facade->contains_procedure_use_var("proc4", "b"));
        REQUIRE_FALSE(read_facade->contains_procedure_use_var(
            "proc2", "u")); // Should not be able to access caller's variables.
        REQUIRE_FALSE(read_facade->contains_procedure_use_var("main", "q")); // Disjoint Call Graph
    }
}