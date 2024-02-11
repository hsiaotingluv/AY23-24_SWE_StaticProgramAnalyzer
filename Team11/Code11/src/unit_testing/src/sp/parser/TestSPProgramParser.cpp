#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/ast/program_ast.hpp"
#include "sp/parser/ast/procedure_ast.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include <memory>

using namespace sp;

TEST_CASE("Test SP Program Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    SECTION("simple program with 1 procedure - success") {
        std::string input = "procedure main {read x;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ProgramParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ProgramNode>(node);
        REQUIRE(node->T == NodeType::Program);
        REQUIRE(casted_node->procedures[0]->T == NodeType::Procedure);
    }

    SECTION("complex program but 1 procedure - success") {
        std::string input = R"(procedure printAscending {
            read num1;
            read num2;
            noSwap = 0;

            if (num1 > num2) then {
              temp = num1;
              num1 = num2;
              num2 = temp;
            } else {
              noSwap = 1;
            }

            print num1;
            print num2;
            print noSwap;
        })";

        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ProgramParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ProgramNode>(node);
        REQUIRE(node->T == NodeType::Program);

        REQUIRE(casted_node->procedures[0]->T == NodeType::Procedure);
        auto first_procedure = std::dynamic_pointer_cast<ProcedureNode>(casted_node->procedures[0]);
        REQUIRE(first_procedure->proc_name == "printAscending");
        REQUIRE(it->T == TokenType::Done);
    }

    SECTION("complex program but multiple procedure - success") {
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

        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ProgramParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ProgramNode>(node);
        REQUIRE(node->T == NodeType::Program);
        REQUIRE(casted_node->procedures.size() == 4);
    }

    SECTION("program with leftover tokens - failure") {
        std::string input = "proc main {read x;}123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ProcedureParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }
}