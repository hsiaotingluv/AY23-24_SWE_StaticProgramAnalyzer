#include "catch.hpp"
#include "common/tokeniser/delimiter.hpp"

using namespace tokenizer;

TEST_CASE("Test LParen") {
    const auto lparen_tokenizer = LParenTokenizer();
    SECTION("lparen - success") {
        const auto result = lparen_tokenizer.tokenize("(a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::LParen);
        REQUIRE(success.content == "(");
        REQUIRE(rest == "a");
    }

    SECTION("letter failure - integer") {
        const auto result = lparen_tokenizer.tokenize("1");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test RParen") {
    const auto rparen_tokenizer = RParenTokenizer();
    SECTION("rparen - success") {
        const auto result = rparen_tokenizer.tokenize(")a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::RParen);
        REQUIRE(success.content == ")");
        REQUIRE(rest == "a");
    }

    SECTION("rparen failure - empty") {
        const auto result = rparen_tokenizer.tokenize("");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Semicolon") {
    const auto semicolon_tokenizer = SemicolonTokenizer();

    SECTION("semicolon - success") {
        const auto result = semicolon_tokenizer.tokenize(";;;");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Semicolon);
        REQUIRE(success.content == ";");
        REQUIRE(rest == ";;");
    }

    SECTION("semicolon fail - random letters") {
        const auto result = semicolon_tokenizer.tokenize("x;");
        REQUIRE(!result.has_value());
    }
}