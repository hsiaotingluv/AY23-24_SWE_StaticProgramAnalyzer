#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/parser/ast/ast.hpp"
#include "sp/parser/statement_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/parser/ast/statement_ast.hpp"
#include <memory>

using namespace sp;

TEST_CASE("Test SP Statement Parser") {
    auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>(), true};
    SECTION("read statement - success") {
        std::string input = "read variable;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<ReadNode>(node);
        REQUIRE(node->T == NodeType::Read);
        REQUIRE(casted_node->variable == "variable");
        REQUIRE(it->T == TokenType::Done);
    }

    SECTION("print statement - success") {
        std::string input = "print variable;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<PrintNode>(node);
        REQUIRE(node->T == NodeType::Print);
        REQUIRE(casted_node->variable == "variable");
        REQUIRE(it->T == TokenType::Done);
    }

    SECTION("call statement - success") {
        std::string input = "call procName;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<CallNode>(node);
        REQUIRE(node->T == NodeType::Call);
        REQUIRE(casted_node->variable == "procName");
        REQUIRE(it->T == TokenType::Done);
    }

    SECTION("assignment statement - success") {
        std::string input = "z = 1;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<AssignmentNode>(node);
        REQUIRE(node->T == NodeType::Assign);
        REQUIRE(casted_node->variable->T == NodeType::Variable);
        REQUIRE(casted_node->expr->T == NodeType::Constant);
    }

    SECTION("if statement - success") {
        std::string input = "if (x < 1) then {z = 1;} else {z = 2;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<IfNode>(node);
        REQUIRE(node->T == NodeType::If);
        REQUIRE(casted_node->cond_expr->T == NodeType::Lt);
        REQUIRE(casted_node->then_stmt_list->T == NodeType::StmtList);
        REQUIRE(casted_node->else_stmt_list->T == NodeType::StmtList);
    }

    SECTION("while statement - success") {
        std::string input = "while (x < 1) {x = x - 1;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<WhileNode>(node);
        REQUIRE(node->T == NodeType::While);
        REQUIRE(casted_node->cond_expr->T == NodeType::Lt);
        REQUIRE(casted_node->stmt_list->T == NodeType::StmtList);
    }

    SECTION("while statement complex conditional and statements - success") {
        std::string input = "while (((x) < 1) && (y > (1))) {x = x + 1; y = y + x + 1 * 2;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        auto node = statement_parser.parse(it, tokens.end());
        auto casted_node = std::dynamic_pointer_cast<WhileNode>(node);
        REQUIRE(node->T == NodeType::While);
        REQUIRE(casted_node->cond_expr->T == NodeType::LAnd);
        REQUIRE(casted_node->stmt_list->T == NodeType::StmtList);
    }

    SECTION("invalid assignment statement integer - failure") {
        std::string input = "1 = 1;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }

    SECTION("invalid print integer - failure") {
        std::string input = "print 123;";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }

    SECTION("capital if statement - failure ") {
        std::string input = "IF (x < 1) then {z = 1;} else {z = 2;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }

    SECTION("if no else statement - failure ") {
        std::string input = "if (x < 1) then {z = 1;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }

    SECTION("if not valid statement in else - failure ") {
        std::string input = "if (x < 1) then {z = 1; x + 1} else {z = 2;}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }

    SECTION("if empty else - failure ") {
        std::string input = "if (x < 1) then {z = 1;} else {}";
        auto tokens = tokenizer_runner.apply_tokeniser(input);
        auto statement_parser = StatementParser();

        auto it = tokens.cbegin();
        REQUIRE_THROWS(statement_parser.parse(it, tokens.end()));
    }
}