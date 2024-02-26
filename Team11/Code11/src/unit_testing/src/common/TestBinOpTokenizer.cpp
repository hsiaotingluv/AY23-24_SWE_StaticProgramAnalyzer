#include "catch.hpp"
#include "common/tokeniser/binop.hpp"

using namespace tokenizer;

TEST_CASE("Test add") {
    const auto add_tokenizer = AddTokenizer();

    SECTION("add success") {
        const auto result = add_tokenizer.tokenize("+x");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Add);
        REQUIRE(success.content == "+");
        REQUIRE(rest == "x");
    }

    SECTION("add failure - letter") {
        const auto result = add_tokenizer.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("add failure - empty") {
        const auto result = add_tokenizer.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("add failure - minus") {
        const auto result = add_tokenizer.tokenize("-");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Subtract") {
    const auto subtract_tokenizer = SubTokenizer();

    SECTION("subtract success") {
        const auto result = subtract_tokenizer.tokenize("-x");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Sub);
        REQUIRE(success.content == "-");
        REQUIRE(rest == "x");
    }

    SECTION("subtract failure - letter") {
        const auto result = subtract_tokenizer.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("subtract failure - empty") {
        const auto result = subtract_tokenizer.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("subtract failure - plus") {
        const auto result = subtract_tokenizer.tokenize("+");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Multiply") {
    const auto multiply_tokenizer = MulTokenizer();

    SECTION("multiply success") {
        const auto result = multiply_tokenizer.tokenize("*x");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Mul);
        REQUIRE(success.content == "*");
        REQUIRE(rest == "x");
    }

    SECTION("multiply failure - letter") {
        const auto result = multiply_tokenizer.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("multiply failure - empty") {
        const auto result = multiply_tokenizer.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("multiply failure - plus") {
        const auto result = multiply_tokenizer.tokenize("+");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Divide") {
    const auto divide_tokenizer = DivTokenizer();

    SECTION("divide success") {
        const auto result = divide_tokenizer.tokenize("/x");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Div);
        REQUIRE(success.content == "/");
        REQUIRE(rest == "x");
    }

    SECTION("divide failure - letter") {
        const auto result = divide_tokenizer.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("divide failure - empty") {
        const auto result = divide_tokenizer.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("divide failure - plus") {
        const auto result = divide_tokenizer.tokenize("+");
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Modulo") {
    const auto modulo_tokenizer = ModTokenizer();

    SECTION("modulo success") {
        const auto result = modulo_tokenizer.tokenize("%x");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Mod);
        REQUIRE(success.content == "%");
        REQUIRE(rest == "x");
    }

    SECTION("modulo failure - letter") {
        const auto result = modulo_tokenizer.tokenize("a");
        REQUIRE(!result.has_value());
    }

    SECTION("modulo failure - empty") {
        const auto result = modulo_tokenizer.tokenize("");
        REQUIRE(!result.has_value());
    }

    SECTION("modulo failure - plus") {
        const auto result = modulo_tokenizer.tokenize("+");
        REQUIRE(!result.has_value());
    }
}
