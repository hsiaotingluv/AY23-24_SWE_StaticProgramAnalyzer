#include "catch.hpp"
#include "common/ast/statement_list_ast.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/statement_list_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include <memory>

using namespace sp;

TEST_CASE("Test SP Statement List Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    SECTION("statements - success") {
        std::string input = "read variable; print variable; call Haha;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<StatementListNode>(node);
        REQUIRE(node->T == NodeType::StmtList);
        REQUIRE(casted_node->statements[0]->T == NodeType::Read);
        REQUIRE(casted_node->statements[1]->T == NodeType::Print);
        REQUIRE(casted_node->statements[2]->T == NodeType::Call);
        REQUIRE(it->T == TokenType::Done);
    }

    SECTION("statements with if - success") {
        std::string input = "read variable; if (x < 1) then {x = 1;} else {x = 2;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<StatementListNode>(node);
        REQUIRE(node->T == NodeType::StmtList);
        REQUIRE(casted_node->statements[0]->T == NodeType::Read);
        REQUIRE(casted_node->statements[1]->T == NodeType::If);
        REQUIRE(it->T == TokenType::Done);
    }

    SECTION("statements with while - success") {
        std::string input = "read variable; while (x < 1) {x = 1;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<StatementListNode>(node);
        REQUIRE(node->T == NodeType::StmtList);
        REQUIRE(casted_node->statements[0]->T == NodeType::Read);
        REQUIRE(casted_node->statements[1]->T == NodeType::While);
        REQUIRE(it->T == TokenType::Done);
    }

    SECTION("statements ended with curly - success") {
        std::string input = "read variable; x = 1;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<StatementListNode>(node);
        REQUIRE(node->T == NodeType::StmtList);
        REQUIRE(casted_node->statements[0]->T == NodeType::Read);
        REQUIRE(casted_node->statements[1]->T == NodeType::Assign);
    }

    SECTION("checking next starting token - success") {
        std::string input = "read variable; x = 1;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<StatementListNode>(node);
        REQUIRE(node->T == NodeType::StmtList);
        REQUIRE(casted_node->statements[0]->T == NodeType::Read);
        REQUIRE(casted_node->statements[1]->T == NodeType::Assign);
        REQUIRE(it->T == TokenType::RCurly);
    }

    SECTION("invalid statement first - failure") {
        std::string input = "1 + 2; x = 1;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }

    SECTION("invalid statement second - failure") {
        std::string input = "x = 1; 1 + 2;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }

    SECTION("invalid statement ending - failure") {
        std::string input = "x = 1; 1 + 2;)";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }

    SECTION("no statement - failure") {
        std::string input;
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementListParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }
}