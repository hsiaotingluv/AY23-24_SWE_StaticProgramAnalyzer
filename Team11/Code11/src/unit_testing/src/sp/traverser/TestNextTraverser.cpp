#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-suspicious-missing-comma"
#include "catch.hpp"

#include "common/ast/statement_ast.hpp"
#include "sp/cfg/cfg_builder.hpp"
#include "sp/main.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/stmt_num_traverser.hpp"
#include "sp/traverser/traverser.hpp"

TEST_CASE("Usability Test on Next Traverser") {
    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);

    SECTION("Complex Program Code - success") {
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

        auto ast = sp->process(input);            // Usability Test. Should not throw any errors.
        REQUIRE(ast->T == sp::NodeType::Program); // This is a placeholder test.
    }

    SECTION("Program Code with Dummy Nodes - success") {
        std::string input = R"(procedure computeCentroid {
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

        auto ast = sp->process(input);            // Usability Test. Should not throw any errors.
        REQUIRE(ast->T == sp::NodeType::Program); // This is a placeholder test.
    }
}

#pragma clang diagnostic pop