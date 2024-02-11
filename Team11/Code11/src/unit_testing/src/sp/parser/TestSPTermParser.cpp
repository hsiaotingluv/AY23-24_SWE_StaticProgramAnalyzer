#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/term_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include <memory>

using namespace sp;

TEST_CASE("Test SP Term Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    SECTION("single factor - success") {
        std::string input = "123 whatever";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = TermParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::static_pointer_cast<ConstantNode>(node);
        REQUIRE(node->T == NodeType::Constant);
        REQUIRE(casted_node->integer == 123);
    }

    SECTION("single binop term - success") {
        std::string input = "123 * variable";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = TermParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::static_pointer_cast<BinaryNode>(node);
        REQUIRE(casted_node->T == NodeType::Mul);
        REQUIRE(casted_node->left->T == NodeType::Constant);
        REQUIRE(casted_node->right->T == NodeType::Variable);
    }

    SECTION("multiple binop terms - success") {
        std::string input = "123 * variable / 5 % randomNumber";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = TermParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::static_pointer_cast<BinaryNode>(node);
        REQUIRE(casted_node->T == NodeType::Mod);

        auto left_node = std::static_pointer_cast<BinaryNode>(casted_node->left);
        REQUIRE(left_node->T == NodeType::Div);
        REQUIRE(left_node->left->T == NodeType::Mul);

        REQUIRE(casted_node->right->T == NodeType::Variable);
    }

    SECTION("multiple binop single term - success") {
        std::string input = "123 + variable / 5 % randomNumber";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = TermParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::static_pointer_cast<ConstantNode>(node);
        REQUIRE(casted_node->T == NodeType::Constant);
        REQUIRE(casted_node->integer == 123);
    }

    SECTION("invalid binop form end - failure") {
        std::string input = "123 **";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = TermParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("invalid binop form beginning - failure") {
        std::string input = "**123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = TermParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("invalid binop form variable - failure") {
        std::string input = "variable**123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = TermParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }
}