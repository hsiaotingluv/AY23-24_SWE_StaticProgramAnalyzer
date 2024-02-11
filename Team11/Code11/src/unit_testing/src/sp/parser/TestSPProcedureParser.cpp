#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/ast/procedure_ast.hpp"
#include "sp/parser/procedure_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include <memory>

using namespace sp;

TEST_CASE("Test SP Procedure Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    SECTION("simple procedure - success") {
        std::string input = "procedure main {read x;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ProcedureParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        REQUIRE(node->T == NodeType::Procedure);
        REQUIRE(casted_node->proc_name == "main");
    }

    SECTION("one procedure with complex statements - success") {
        std::string input = "procedure print {read x; while (x < 10) {print x;}}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ProcedureParser();

        auto it = tokens.cbegin();
        auto node = term_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ProcedureNode>(node);
        REQUIRE(node->T == NodeType::Procedure);
        REQUIRE(casted_node->proc_name == "print");
    }

    SECTION("typo keywords procedure - success") {
        std::string input = "proc main {read x;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto term_parser = ProcedureParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(term_parser.parse(it, tokens.end()));
    }
}