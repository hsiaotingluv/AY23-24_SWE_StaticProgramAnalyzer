#include "catch.hpp"
#include "common/ast/node_type.hpp"
#include "sp/main.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/stmt_num_traverser.hpp"
#include "sp/traverser/cfg_builder.hpp"

TEST_CASE("Test SP") {
    auto tokenizer_runner =
        std::make_shared<tokenizer::TokenizerRunner>(std::make_unique<sp::SourceProcessorTokenizer>(), true);
    auto parser = std::make_shared<sp::ProgramParser>();
    auto [read_facade, write_facade] = PKB::create_facades();
    auto cfg_builder = std::make_shared<sp::CfgBuilder>();
    std::vector<std::shared_ptr<sp::Traverser>> traversers = {std::make_shared<sp::StmtNumTraverser>(write_facade)};
    auto sp = sp::SourceProcessor{tokenizer_runner, parser, cfg_builder, traversers};

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

TEST_CASE("Test SP Xml") {
    auto tokenizer_runner =
        std::make_shared<tokenizer::TokenizerRunner>(std::make_unique<sp::SourceProcessorTokenizer>(), true);
    auto parser = std::make_shared<sp::ProgramParser>();
    auto [read_facade, write_facade] = PKB::create_facades();
    auto cfg_builder = std::make_shared<sp::CfgBuilder>();
    std::vector<std::shared_ptr<sp::Traverser>> traversers = {std::make_shared<sp::StmtNumTraverser>(write_facade)};
    auto sp = sp::SourceProcessor{tokenizer_runner, parser, cfg_builder, traversers};

    SECTION("Simple variable - success") {
        std::string input = R"(procedure main {
            x = 1;
        })";
        auto ast = sp.process(input);
        auto ast_string = sp::SourceProcessor::output_xml(ast);
        REQUIRE(
            ast_string ==
            R"(<ProgramNode><ProcedureNode name="main"><StatementListNode><AssignmentNode stmt_number="1"><VarNode name="x" /><ConstantNode integer="1" /></AssignmentNode></StatementListNode></ProcedureNode></ProgramNode>)");
    }

    SECTION("Complex Code - success") {
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
        auto ast_string = sp::SourceProcessor::output_xml(ast);
        REQUIRE(
            ast_string ==
            R"(<ProgramNode><ProcedureNode name="main"><StatementListNode><AssignmentNode stmt_number="1"><VarNode name="flag" /><ConstantNode integer="0" /></AssignmentNode><CallNode stmt_number="2" proc_name="computeCentroid" /><CallNode stmt_number="3" proc_name="printResults" /></StatementListNode></ProcedureNode><ProcedureNode name="readPoint"><StatementListNode><ReadNode stmt_number="4"><VarNode name="x" /></ReadNode><ReadNode stmt_number="5"><VarNode name="y" /></ReadNode></StatementListNode></ProcedureNode><ProcedureNode name="printResults"><StatementListNode><PrintNode stmt_number="6"><VarNode name="flag" /></PrintNode><PrintNode stmt_number="7"><VarNode name="cenX" /></PrintNode><PrintNode stmt_number="8"><VarNode name="cenY" /></PrintNode><PrintNode stmt_number="9"><VarNode name="normSq" /></PrintNode></StatementListNode></ProcedureNode><ProcedureNode name="computeCentroid"><StatementListNode><AssignmentNode stmt_number="10"><VarNode name="count" /><ConstantNode integer="0" /></AssignmentNode><AssignmentNode stmt_number="11"><VarNode name="cenX" /><ConstantNode integer="0" /></AssignmentNode><AssignmentNode stmt_number="12"><VarNode name="cenY" /><ConstantNode integer="0" /></AssignmentNode><CallNode stmt_number="13" proc_name="readPoint" /><WhileNode stmt_number="14"><LogicalAndNode><NotEqualNode><VarNode name="x" /><ConstantNode integer="0" /></NotEqualNode><NotEqualNode><VarNode name="y" /><ConstantNode integer="0" /></NotEqualNode></LogicalAndNode><StatementListNode><AssignmentNode stmt_number="15"><VarNode name="count" /><AddNode><VarNode name="count" /><ConstantNode integer="1" /></AddNode></AssignmentNode><AssignmentNode stmt_number="16"><VarNode name="cenX" /><AddNode><VarNode name="cenX" /><VarNode name="x" /></AddNode></AssignmentNode><AssignmentNode stmt_number="17"><VarNode name="cenY" /><AddNode><VarNode name="cenY" /><VarNode name="y" /></AddNode></AssignmentNode><CallNode stmt_number="18" proc_name="readPoint" /></StatementListNode></WhileNode><IfNode stmt_number="19"><EqualNode><VarNode name="count" /><ConstantNode integer="0" /></EqualNode><StatementListNode><AssignmentNode stmt_number="20"><VarNode name="flag" /><ConstantNode integer="1" /></AssignmentNode></StatementListNode><StatementListNode><AssignmentNode stmt_number="21"><VarNode name="cenX" /><DivNode><VarNode name="cenX" /><VarNode name="count" /></DivNode></AssignmentNode><AssignmentNode stmt_number="22"><VarNode name="cenY" /><DivNode><VarNode name="cenY" /><VarNode name="count" /></DivNode></AssignmentNode></StatementListNode></IfNode><AssignmentNode stmt_number="23"><VarNode name="normSq" /><AddNode><MulNode><VarNode name="cenX" /><VarNode name="cenX" /></MulNode><MulNode><VarNode name="cenY" /><VarNode name="cenY" /></MulNode></AddNode></AssignmentNode></StatementListNode></ProcedureNode></ProgramNode>)");
    }
}
