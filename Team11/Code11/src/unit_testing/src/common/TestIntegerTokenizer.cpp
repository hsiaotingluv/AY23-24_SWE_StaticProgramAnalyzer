#include "catch.hpp"
#include "common/tokeniser/integer.hpp"

using namespace tokenizer;

TEST_CASE("Test zero") {
    const auto zero_tokenizer = ZeroTokenizer();

    SECTION("zero success") {
        const auto result = zero_tokenizer.tokenize("0");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Integer);
        REQUIRE(success.content == "0");
        REQUIRE(rest == "");
    }

    SECTION("zero failure - letter") {
        const auto result = zero_tokenizer.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("zero failure - empty") {
        const auto result = zero_tokenizer.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("zero failure - non-zero digit") {
        const auto result = zero_tokenizer.tokenize("1");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test nzdigit") {
    const auto nzdigit = NotZeroDigitTokenizer();
    SECTION("nzdigit success") {
        const auto result = nzdigit.tokenize("1");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Integer);
        REQUIRE(success.content == "1");
        REQUIRE(rest == "");
    }

    SECTION("nzdigit failure - letter") {
        const auto result = nzdigit.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("nzdigit failure - empty") {
        const auto result = nzdigit.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("nzdigit failure - zero") {
        const auto result = nzdigit.tokenize("0");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test digit") {
    const auto digit = DigitTokenizer();
    SECTION("digit success - zero") {
        const auto result = digit.tokenize("0");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Integer);
        REQUIRE(success.content == "0");
        REQUIRE(rest == "");
    }

    SECTION("digit success - non-zero") {
        const auto result = digit.tokenize("12");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Integer);
        REQUIRE(success.content == "1");
        REQUIRE(rest == "2");
    }

    SECTION("digit failure - letter") {
        const auto result = digit.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("digit failure - empty") {
        const auto result = digit.tokenize("");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Integer") {
    SECTION("Integer success") {
        auto integer_tokenizer = IntegerTokenizer();
        auto result = integer_tokenizer.tokenize("123 ");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Integer);
        REQUIRE(success.content == "123");
        REQUIRE(rest == " ");
    }

    SECTION("integer 0 - success") {
        auto integer_tokenizer = IntegerTokenizer();
        auto result = integer_tokenizer.tokenize("0");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Integer);
        REQUIRE(success.content == "0");
        REQUIRE(rest == "");
    }

    SECTION("integer failure - letter") {
        auto integer_tokenizer = IntegerTokenizer();
        auto result = integer_tokenizer.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("integer failure - empty") {
        auto integer_tokenizer = IntegerTokenizer();
        auto result = integer_tokenizer.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("integer negative - failure") {
        auto integer_tokenizer = IntegerTokenizer();
        auto result = integer_tokenizer.tokenize("-123");
        REQUIRE(!result.has_value());
    }
}
