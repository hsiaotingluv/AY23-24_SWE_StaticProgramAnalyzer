#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/ast/logical_ast.hpp"
#include "sp/parser/ast/rel_expr_ast.hpp"
#include "sp/parser/cond_expr_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include <memory>

using namespace sp;

TEST_CASE("Test SP Cond Term Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    SECTION("single relational - success") {
        std::string input = "(1) < (2)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto cond_expr_parser = CondExprParser();

        auto it = tokens.cbegin();
        auto node = cond_expr_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ComparatorNode>(node);
        REQUIRE(node->T == NodeType::Lt);
        REQUIRE(casted_node->left->T == NodeType::Constant);
        REQUIRE(casted_node->right->T == NodeType::Constant);
    }

    SECTION("not simple relational - success") {
        std::string input = "!(1 < 2)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto cond_expr_parser = CondExprParser();

        auto it = tokens.cbegin();
        auto node = cond_expr_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<LogicalNotNode>(node);
        REQUIRE(node->T == NodeType::LNot);
        REQUIRE(casted_node->cond_expr->T == NodeType::Lt);
    }

    SECTION("simple && conditional - success") {
        std::string input = "(1 < 2) && (3 < 4)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto cond_expr_parser = CondExprParser();

        auto it = tokens.cbegin();
        auto node = cond_expr_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<LogicalBinaryNode>(node);
        REQUIRE(node->T == NodeType::LAnd);
        REQUIRE(casted_node->left->T == NodeType::Lt);
        REQUIRE(casted_node->right->T == NodeType::Lt);
    }

    SECTION("simple || conditional - success") {
        std::string input = "(1 < 2) || (3 < 4)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto cond_expr_parser = CondExprParser();

        auto it = tokens.cbegin();
        auto node = cond_expr_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<LogicalBinaryNode>(node);
        REQUIRE(node->T == NodeType::LOr);
        REQUIRE(casted_node->left->T == NodeType::Lt);
        REQUIRE(casted_node->right->T == NodeType::Lt);
    }

    SECTION("edge case cond expr vs rel expr - success") {
        std::string input = "((1) < (2)) || ((((3))) < (4))";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto cond_expr_parser = CondExprParser();

        auto it = tokens.cbegin();
        auto node = cond_expr_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<LogicalBinaryNode>(node);
        REQUIRE(node->T == NodeType::LOr);
        REQUIRE(casted_node->left->T == NodeType::Lt);
        REQUIRE(casted_node->right->T == NodeType::Lt);
        REQUIRE(it->T == TokenType::Done);
    }

    SECTION("edge case tons of brackets - success") {
        std::string input = "((1)) < ((2))";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto cond_expr_parser = CondExprParser();

        auto it = tokens.cbegin();
        auto node = cond_expr_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<LogicalBinaryNode>(node);
        REQUIRE(node->T == NodeType::Lt);
    }

    SECTION("invalid duplicate operator - failure") {
        std::string input = "(variable > 2) ||&& (123 > 1)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = CondExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("invalid rel expr - failure") {
        std::string input = "(1 < 2)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = CondExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("invalid missing brackets, partial parsing - failure") {
        std::string input = "variable > 2 || 123 > 1";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = CondExprParser();

        auto it = tokens.cbegin();
        auto t = term_parser.parse(it, tokens.end());
        REQUIRE(t->T == NodeType::Gt);
        // Unfinished parsing, only parse the left hand side
        REQUIRE_FALSE(it->T == TokenType::Done);
    }
}