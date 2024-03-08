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
    auto tokenizer_runner =std::make_shared<tokenizer::TokenizerRunner>(std::make_unique<sp::SourceProcessorTokenizer>(), true);
    auto parser = std::make_shared<sp::ProgramParser>();
    auto [read_facade, write_facade] = PKB::create_facades();
    auto cfg_builder = std::make_shared<sp::CfgBuilder>();
    auto stmt_num_traverser = std::make_shared<sp::StmtNumTraverser>(write_facade);
    std::vector<std::shared_ptr<sp::Traverser>> design_abstr_traversers = {};
    auto next_traverser = std::make_shared<sp::NextTraverser>(write_facade);
    auto sp = sp::SourceProcessor{tokenizer_runner,        parser,        stmt_num_traverser, cfg_builder,
                                  design_abstr_traversers, next_traverser};

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

        auto ast = sp.process(input);            // Usability Test. Should not throw any errors.
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

        auto ast = sp.process(input);            // Usability Test. Should not throw any errors.
        REQUIRE(ast->T == sp::NodeType::Program); // This is a placeholder test.
    }
}

#pragma clang diagnostic pop