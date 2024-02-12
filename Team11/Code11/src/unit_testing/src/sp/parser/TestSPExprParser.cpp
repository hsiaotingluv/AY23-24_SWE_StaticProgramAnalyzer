#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/ast/binary_node_ast.hpp"
#include "sp/parser/ast/factor_ast.hpp"
#include "sp/parser/expr_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include <memory>

using namespace sp;

TEST_CASE("Test SP Expr Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    SECTION("single term - success") {
        std::string input = "123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ConstantNode>(node);
        REQUIRE(node->T == NodeType::Constant);
        REQUIRE(casted_node->integer == 123);
    }

    SECTION("single expression with single binop - success") {
        std::string input = "123 + variable";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<BinopNode>(node);
        REQUIRE(casted_node->T == NodeType::Add);
        REQUIRE(casted_node->left->T == NodeType::Constant);
        REQUIRE(casted_node->right->T == NodeType::Variable);
    }

    SECTION("multiple expressions - success") {
        std::string input = "1 + 2 - variable * 3";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<BinopNode>(node);
        REQUIRE(casted_node->T == NodeType::Sub);
        REQUIRE(casted_node->right->T == NodeType::Mul);

        auto left_node = std::dynamic_pointer_cast<BinopNode>(casted_node->left);
        REQUIRE(left_node->T == NodeType::Add);
        REQUIRE(left_node->left->T == NodeType::Constant);
        REQUIRE(left_node->right->T == NodeType::Constant);
    }

    SECTION("multiple complex expressions - success") {
        std::string input = "123 * variable + variable % 20 - last";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<BinopNode>(node);
        REQUIRE(casted_node->T == NodeType::Sub);

        auto left_node = std::dynamic_pointer_cast<BinopNode>(casted_node->left);
        REQUIRE(left_node->T == NodeType::Add);
        REQUIRE(left_node->left->T == NodeType::Mul);
        REQUIRE(left_node->right->T == NodeType::Mod);
    }

    SECTION("multiple expressions brackets - success") {
        std::string input = "(2 + variable) * 2";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<BinopNode>(node);
        REQUIRE(casted_node->T == NodeType::Mul);

        auto left_node = std::dynamic_pointer_cast<BinopNode>(casted_node->left);
        REQUIRE(left_node->T == NodeType::Add);
        REQUIRE(left_node->left->T == NodeType::Constant);
        REQUIRE(left_node->right->T == NodeType::Variable);
    }

    SECTION("multiple recursive brackets - success") {
        std::string input = "(((2 + 3)))";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<BinopNode>(node);
        REQUIRE(casted_node->T == NodeType::Add);
    }

    SECTION("invalid expr form end - failure") {
        std::string input = "123++";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("invalid expr form beginning - failure") {
        std::string input = "--123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("invalid expr variable - failure") {
        std::string input = "variable**123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("unbalanced brackets - failure") {
        std::string input = "(((2 + 3))";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }
}