#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/ast/factor_ast.hpp"
#include "sp/parser/constant_parser.hpp"
#include "sp/parser/factor_parser.hpp"
#include "sp/parser/name_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include <memory>
#include <string>

using namespace sp;

TEST_CASE("Test SP Factor Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};

    SECTION("integer parser - success") {
        std::string input = "123 whatever";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto constant_parser = ConstantParser();

        // Note: https://stackoverflow.com/questions/27463785/cant-pass-temporary-object-as-reference
        auto it = tokens.cbegin();
        auto node = constant_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ConstantNode>(node);
        REQUIRE(node->T == NodeType::Constant);
        REQUIRE(casted_node->integer == 123);
    }

    SECTION("name parser - success") {
        std::string input = "whatever 123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto constant_parser = NameParser();

        auto it = tokens.cbegin();
        auto node = constant_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<NameNode>(node);
        REQUIRE(node->T == NodeType::Variable);
        REQUIRE(casted_node->name == "whatever");
    }

    SECTION("factor parser name - success") {
        std::string input = "whatever";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto factor_parser = FactorParser();

        auto it = tokens.cbegin();
        auto node = factor_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<NameNode>(node);
        REQUIRE(node->T == NodeType::Variable);
        REQUIRE(casted_node->name == "whatever");
    }

    SECTION("factor parser constant - success") {
        std::string input = "123 123";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto factor_parser = FactorParser();

        auto it = tokens.cbegin();
        auto node = factor_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ConstantNode>(node);
        REQUIRE(node->T == NodeType::Constant);
        REQUIRE(casted_node->integer == 123);
    }

    SECTION("factor parser unknown token - fail") {
        std::string input = "{123}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto factor_parser = FactorParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(factor_parser.parse(it, tokens.end()));
    }

    SECTION("factor expr parser - success") {
        std::string input = "(whatever)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto factor_parser = FactorParser();

        auto it = tokens.cbegin();
        auto node = factor_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<NameNode>(node);
        REQUIRE(node->T == NodeType::Variable);
        REQUIRE(casted_node->name == "whatever");
    }
}
