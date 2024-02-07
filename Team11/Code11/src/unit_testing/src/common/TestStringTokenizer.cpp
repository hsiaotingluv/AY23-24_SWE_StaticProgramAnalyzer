#include "catch.hpp"
#include "common/tokeniser/string.hpp"

using namespace tokenizer;

TEST_CASE("Test letter") {
    const auto letter_tokenizer = LetterTokenizer();
    SECTION("letter success") {
        const auto result = letter_tokenizer.tokenize("a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::String);
        REQUIRE(success.content == "a");
        REQUIRE(rest == "");
    }

    SECTION("letter failure - integer") {
        const auto result = letter_tokenizer.tokenize("1");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test digit or letter") {
    const auto digit_or_letter = DigitOrLetterTokenizer();
    SECTION("digit or letter success - digit") {
        const auto result = digit_or_letter.tokenize("0x");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Integer);
        REQUIRE(success.content == "0");
        REQUIRE(rest == "x");
    }

    SECTION("digit or letter success - letter") {
        const auto result = digit_or_letter.tokenize("a0");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::String);
        REQUIRE(success.content == "a");
        REQUIRE(rest == "0");
    }

    SECTION("digit or letter failure - empty") {
        const auto result = digit_or_letter.tokenize("");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Some Digit or Letter") {
    const auto some_digit_or_letter = SomeDigitOrLetterTokenizer();

    SECTION("some_digit_or_letter success - empty") {
        const auto result = some_digit_or_letter.tokenize("");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::String);
        REQUIRE(success.content == "");
        REQUIRE(rest == "");
    }

    SECTION("some_digit_or_letter success - digit and letter") {
        const auto result = some_digit_or_letter.tokenize("0x_");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::String);
        REQUIRE(success.content == "0x");
        REQUIRE(rest == "_");
    }
}

TEST_CASE("Test string") {
    const auto string = StringTokenizer();
    SECTION("string success - letter") {
        const auto result = string.tokenize("a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::String);
        REQUIRE(success.content == "a");
        REQUIRE(rest == "");
    }

    SECTION("string success - letter and digit") {
        const auto result = string.tokenize("a1b2c3d4e5f6g7h8i9j0k+");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::String);
        REQUIRE(success.content == "a1b2c3d4e5f6g7h8i9j0k");
        REQUIRE(rest == "+");
    }

    SECTION("string failure - empty") {
        const auto result = string.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("string failure - digit") {
        const auto result = string.tokenize("1");
        REQUIRE(!result.has_value());
    }

    SECTION("string failure - starts with digit") {
        const auto result = string.tokenize("1adsfadsf2");
        REQUIRE(!result.has_value());
    }
}