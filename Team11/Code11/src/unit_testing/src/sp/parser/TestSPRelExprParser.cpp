#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/rel_expr_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include <memory>

using namespace sp;

TEST_CASE("Test SP RelExpr Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    SECTION("single relational with simple factor - success") {
        std::string input = "1 > 2";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = RelExprParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::static_pointer_cast<BinaryNode>(node);
        REQUIRE(node->T == NodeType::Gt);
        REQUIRE(casted_node->left->T == NodeType::Constant);
        REQUIRE(casted_node->right->T == NodeType::Constant);
    }

    SECTION("single relational with complex terms - success") {
        std::string input = "(whatever * 2 + 3) == (2 / 3 % variable)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = RelExprParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::static_pointer_cast<BinaryNode>(node);
        REQUIRE(casted_node->T == NodeType::Eq);
        REQUIRE(casted_node->left->T == NodeType::Add);
        REQUIRE(casted_node->right->T == NodeType::Mod);
    }

    SECTION("unknown relational token - failure") {
        std::string input = "123 + 2";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = RelExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("missing one operand - failure") {
        std::string input = ">123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = RelExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }

    SECTION("missing both operands - failure") {
        std::string input = "!=";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = RelExprParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }
}