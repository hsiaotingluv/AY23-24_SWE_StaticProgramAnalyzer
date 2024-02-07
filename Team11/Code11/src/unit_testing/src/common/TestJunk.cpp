#include "catch.hpp"
#include "common/tokeniser/junk.hpp"

using namespace tokenizer;

TEST_CASE("Test Junk") {
    const auto junk_tokenizer = JunkTokenizer();
    SECTION("newline - success") {
        const auto result = junk_tokenizer.tokenize("\n");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Junk);
        REQUIRE(success.content == "\n");
        REQUIRE(rest == "");
    }

    SECTION("tab - success") {
        const auto result = junk_tokenizer.tokenize("\t");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Junk);
        REQUIRE(success.content == "\t");
        REQUIRE(rest == "");
    }

    SECTION("spaces - success") {
        const auto result = junk_tokenizer.tokenize("  ");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::Junk);
        REQUIRE(success.content == " ");
        REQUIRE(rest == " ");
    }

    SECTION("not junk failed - valid letters") {
        const auto result = junk_tokenizer.tokenize("abc");
        REQUIRE(!result.has_value());
    }
}