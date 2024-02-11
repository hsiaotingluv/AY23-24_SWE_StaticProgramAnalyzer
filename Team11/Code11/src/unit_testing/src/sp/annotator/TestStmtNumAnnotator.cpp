#include "catch.hpp"

#include "sp/annotator/stmt_num_annotator.hpp"
#include "sp/main.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"

#include <cstdint>
#include <vector>

class DummyAnnotator : public sp::Annotator {
  public:
    auto annotate(std::shared_ptr<sp::AstNode> ast) -> std::shared_ptr<sp::AstNode> override {
        return ast;
    }
};

auto is_stmt_node(std::shared_ptr<sp::AstNode> node) -> bool {
    return (dynamic_cast<sp::StatementNode*>(node.get()) != nullptr);
}

auto require_stmt_num_populated(std::shared_ptr<sp::AstNode> node) -> bool {
    if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<sp::StatementNode>(node);
        if (!stmt_node->has_statement_number()) {
            return false;
        }
    }

    for (const auto& child : node->get_children()) {
        if (!require_stmt_num_populated(child)) {
            return false;
        }
    }
    return true;
}

auto check_stmt_num(std::shared_ptr<sp::AstNode> node, uint32_t stmt_num,
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

auto print_stmt_num(std::shared_ptr<sp::AstNode> node) -> void {
    if (is_stmt_node(node)) {
        auto stmt_node = std::dynamic_pointer_cast<sp::StatementNode>(node);
        std::cout << *node << std::endl;
    }

    for (const auto& child : node->get_children()) {
        print_stmt_num(child);
    }
}

TEST_CASE("Test Statement Number Annotator") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    auto parser = std::make_shared<sp::ProgramParser>();
    auto sp = sp::SourceProcessor{tokenizer_runner, parser, std::make_shared<DummyAnnotator>()};

    const auto stmt_num_annotator = std::make_shared<sp::StmtNumAnnotator>();

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

        auto ast = sp.parse(input);
        ast = stmt_num_annotator->annotate(ast);
        REQUIRE(require_stmt_num_populated(ast));

        const auto expected_output = std::vector<std::string>{
            "AssignmentNode(NameNode(flag), ConstantNode(0))",
            "CallNode(computeCentroid)",
            "CallNode(printResults)",
            "ReadNode(x)",
            "ReadNode(y)",
            "PrintNode(flag)",
            "PrintNode(cenX)",
            "PrintNode(cenY)",
            "PrintNode(normSq)",
            "AssignmentNode(NameNode(count), ConstantNode(0))",
            "AssignmentNode(NameNode(cenX), ConstantNode(0))",
            "AssignmentNode(NameNode(cenY), ConstantNode(0))",
            "CallNode(readPoint)",
            "WhileNode(LogicalAndNode(NotEqualNode(NameNode(x), ConstantNode(0)), NotEqualNode(NameNode(y), "
            "ConstantNode(0))), StatementListNode(AssignmentNode(NameNode(count), BinaryNode(NameNode(count), "
            "ConstantNode(1))), AssignmentNode(NameNode(cenX), BinaryNode(NameNode(cenX), NameNode(x))), "
            "AssignmentNode(NameNode(cenY), BinaryNode(NameNode(cenY), NameNode(y))), CallNode(readPoint), ))",
            "AssignmentNode(NameNode(count), BinaryNode(NameNode(count), ConstantNode(1)))",
            "AssignmentNode(NameNode(cenX), BinaryNode(NameNode(cenX), NameNode(x)))",
            "AssignmentNode(NameNode(cenY), BinaryNode(NameNode(cenY), NameNode(y)))",
            "CallNode(readPoint)",
            "IfNode(EqualNode(NameNode(count), ConstantNode(0)), "
            "StatementListNode(AssignmentNode(NameNode(flag), ConstantNode(1)), ), "
            "StatementListNode(AssignmentNode(NameNode(cenX), BinaryNode(NameNode(cenX), NameNode(count))), "
            "AssignmentNode(NameNode(cenY), BinaryNode(NameNode(cenY), NameNode(count))), ))",
            "AssignmentNode(NameNode(flag), ConstantNode(1))",
            "AssignmentNode(NameNode(cenX), BinaryNode(NameNode(cenX), NameNode(count)))",
            "AssignmentNode(NameNode(cenY), BinaryNode(NameNode(cenY), NameNode(count)))",
            "AssignmentNode(NameNode(normSq), BinaryNode(BinaryNode(NameNode(cenX), NameNode(cenX)), "
            "BinaryNode(NameNode(cenY), NameNode(cenY))))"};

        check_stmt_num(ast, 1, expected_output.begin(), expected_output.end());
    }
}