#include "catch.hpp"
#include "sp/main.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/stmt_num_traverser.hpp"
#include "sp/validator/semantic_validator.hpp"

TEST_CASE("Test SP Semantic Validator") {
    auto tokenizer_runner =
        std::make_shared<tokenizer::TokenizerRunner>(std::make_unique<sp::SourceProcessorTokenizer>(), true);
    auto parser = std::make_shared<sp::ProgramParser>();
    auto [_, write_facade] = PkbManager::create_facades();
    std::vector<std::shared_ptr<sp::Traverser>> traversers = {std::make_shared<sp::StmtNumTraverser>(write_facade)};
    auto sp = sp::SourceProcessor{tokenizer_runner, parser, traversers};
    auto semantic_validator = sp::SemanticValidator();

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
        auto orders = semantic_validator.validate_get_traversal_order(ast);
        REQUIRE((orders[0] == "printResults" || orders[0] == "readPoint"));
        REQUIRE(orders[2] == "computeCentroid");
        REQUIRE(orders[3] == "main");
    }

    SECTION("cycle - failure") {
        std::string input = R"(procedure main {
            call printResults;
        }

        procedure printResults {
            call main;
        })";

        REQUIRE_THROWS(sp.process(input));
    }

    SECTION("recursion - failure") {
        std::string input = R"(procedure main {
            call main;
        })";

        REQUIRE_THROWS(sp.process(input));
    }

    SECTION("non existent - failure") {
        std::string input = R"(procedure main {
            call random;
        })";

        REQUIRE_THROWS(sp.process(input));
    }

    SECTION("duplicate name - failure") {
        std::string input = R"(procedure main {
            x = 1;
        }

        procedure main {
            x = 2;
        })";

        REQUIRE_THROWS(sp.process(input));
    }

    SECTION("cyclic chain calls - failure") {
        std::string input = R"(procedure main {
            call printResults;
        }

        procedure printResults {
            call x;
        }

        procedure x {
            call main;
        })";

        REQUIRE_THROWS(sp.process(input));
    }
}
