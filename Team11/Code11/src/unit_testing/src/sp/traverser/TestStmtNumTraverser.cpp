#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-suspicious-missing-comma"
#include "catch.hpp"

#include "sp/main.hpp"
#include "sp/parser/ast/statement_ast.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/stmt_num_traverser.hpp"
#include "sp/traverser/traverser.hpp"
#include <vector>

auto is_stmt_node(const std::shared_ptr<sp::AstNode>& node) -> bool {
    return (dynamic_cast<sp::StatementNode*>(node.get()) != nullptr);
}

auto require_stmt_num_populated(const std::shared_ptr<sp::AstNode>& node) -> bool {
    if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<sp::StatementNode>(node);
        if (!stmt_node->has_statement_number()) {
            return false;
        }
    }

    auto children = node->get_children();
    return std::all_of(children.begin(), children.end(), [](auto child) {
        return require_stmt_num_populated(child);
    });
}

auto check_stmt_num(const std::shared_ptr<sp::AstNode>& node, uint32_t stmt_num,
                    std::vector<std::string>::const_iterator begin, const std::vector<std::string>::const_iterator end)
    -> std::tuple<std::vector<std::string>::const_iterator, uint32_t> {
    if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<sp::StatementNode>(node);
        std::stringstream ss;
        ss << *node;

        if (begin == end) {
            FAIL("More statement numbers than expected");
        }
        REQUIRE(ss.str() == *begin);
        REQUIRE(stmt_node->get_statement_number() == stmt_num);
        begin = std::next(begin);
        stmt_num += 1;
    }

    for (const auto& child : node->get_children()) {
        const auto results = check_stmt_num(child, stmt_num, begin, end);
        begin = std::get<0>(results);
        stmt_num = std::get<1>(results);
    }
    return {begin, stmt_num};
}

auto print_stmt_num(const std::shared_ptr<sp::AstNode>& node) -> void {
    if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<sp::StatementNode>(node);
        std::cout << *node << std::endl;
    }

    for (const auto& child : node->get_children()) {
        print_stmt_num(child);
    }
}

TEST_CASE("Test Statement Number Traverser") {
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
        REQUIRE(require_stmt_num_populated(ast));

        const auto expected_output = std::vector<std::string>{
            "AssignmentNode(VarNode(flag), ConstantNode(0))",
            "CallNode(computeCentroid)",
            "CallNode(printResults)",
            "ReadNode(x)",
            "ReadNode(y)",
            "PrintNode(flag)",
            "PrintNode(cenX)",
            "PrintNode(cenY)",
            "PrintNode(normSq)",
            "AssignmentNode(VarNode(count), ConstantNode(0))",
            "AssignmentNode(VarNode(cenX), ConstantNode(0))",
            "AssignmentNode(VarNode(cenY), ConstantNode(0))",
            "CallNode(readPoint)",
            "WhileNode(LogicalAndNode(NotEqualNode(VarNode(x), ConstantNode(0)), NotEqualNode(VarNode(y), "
            "ConstantNode(0))), StatementListNode(AssignmentNode(VarNode(count), AddBinopNode(VarNode(count), "
            "ConstantNode(1))), AssignmentNode(VarNode(cenX), AddBinopNode(VarNode(cenX), VarNode(x))), "
            "AssignmentNode(VarNode(cenY), AddBinopNode(VarNode(cenY), VarNode(y))), CallNode(readPoint), ))",
            "AssignmentNode(VarNode(count), AddBinopNode(VarNode(count), ConstantNode(1)))",
            "AssignmentNode(VarNode(cenX), AddBinopNode(VarNode(cenX), VarNode(x)))",
            "AssignmentNode(VarNode(cenY), AddBinopNode(VarNode(cenY), VarNode(y)))",
            "CallNode(readPoint)",
            "IfNode(EqualNode(VarNode(count), ConstantNode(0)), "
            "StatementListNode(AssignmentNode(VarNode(flag), ConstantNode(1)), ), "
            "StatementListNode(AssignmentNode(VarNode(cenX), DivBinopNode(VarNode(cenX), VarNode(count))), "
            "AssignmentNode(VarNode(cenY), DivBinopNode(VarNode(cenY), VarNode(count))), ))",
            "AssignmentNode(VarNode(flag), ConstantNode(1))",
            "AssignmentNode(VarNode(cenX), DivBinopNode(VarNode(cenX), VarNode(count)))",
            "AssignmentNode(VarNode(cenY), DivBinopNode(VarNode(cenY), VarNode(count)))",
            "AssignmentNode(VarNode(normSq), AddBinopNode(MulBinopNode(VarNode(cenX), VarNode(cenX)), "
            "MulBinopNode(VarNode(cenY), VarNode(cenY))))"};

        check_stmt_num(ast, 1, expected_output.begin(), expected_output.end());
    }
}

#pragma clang diagnostic pop