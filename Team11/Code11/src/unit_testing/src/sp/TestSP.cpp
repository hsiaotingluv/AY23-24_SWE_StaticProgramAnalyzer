#include "catch.hpp"
#include "sp/main.hpp"
#include "sp/parser/ast/node_type.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/stmt_num_traverser.hpp"

TEST_CASE("Test SP") {
    auto tokenizer_runner =
        std::make_shared<tokenizer::TokenizerRunner>(std::make_unique<sp::SourceProcessorTokenizer>(), true);
    auto parser = std::make_shared<sp::ProgramParser>();
    std::vector<std::shared_ptr<sp::Traverser>> traversers = {std::make_shared<sp::StmtNumTraverser>()};
    auto sp = sp::SourceProcessor{tokenizer_runner, parser, traversers};

    SECTION("complex program Code 4 - success") {
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

        auto ast = sp.process(input);
        REQUIRE(ast->T == sp::NodeType::Program);
    }
}