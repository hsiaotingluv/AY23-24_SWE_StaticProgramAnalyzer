#include "catch.hpp"
#include "sp/tokeniser/relational.hpp"

using namespace tokenizer;

TEST_CASE("Test Logical Single") {
    const auto sp_relational_tokenizer = SPRelationalTokenizer();
    SECTION("single lt success") {
        const auto result = sp_relational_tokenizer.tokenize("< a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::LessThan);
        REQUIRE(success.content == "<");
        REQUIRE(rest == " a");
    }

    SECTION("single double equal - success") {
        const auto result = sp_relational_tokenizer.tokenize("==a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::DoubleEqual);
        REQUIRE(success.content == "==");
        REQUIRE(rest == "a");
    }

    SECTION("single less than equal - success") {
        const auto result = sp_relational_tokenizer.tokenize("<=a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::LessThanEqual);
        REQUIRE(success.content == "<=");
        REQUIRE(rest == "a");
    }

    SECTION("single greater than equal - success") {
        const auto result = sp_relational_tokenizer.tokenize(">=a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::GreaterThanEqual);
        REQUIRE(success.content == ">=");
        REQUIRE(rest == "a");
    }

    SECTION("single greater than - success") {
        const auto result = sp_relational_tokenizer.tokenize(">");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::GreaterThan);
        REQUIRE(success.content == ">");
        REQUIRE(rest.empty());
    }

    SECTION("not equal - success") {
        const auto result = sp_relational_tokenizer.tokenize("!=");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::NotEqual);
        REQUIRE(success.content == "!=");
        REQUIRE(rest.empty());
    }

    SECTION("relational failure - conditional expression") {
        const auto result = sp_relational_tokenizer.tokenize("!");
        REQUIRE(!result.has_value());
    }
}
