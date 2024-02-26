#include "catch.hpp"
#include "sp/tokeniser/delimiter.hpp"

using namespace tokenizer;

TEST_CASE("Test Delimiter Single") {
    const auto sp_delimiter_tokenizer = SPDelimiterTokenizer();
    SECTION("lcurly success") {
        const auto result = sp_delimiter_tokenizer.tokenize("{a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::LCurly);
        REQUIRE(success.content == "{");
        REQUIRE(rest == "a");
    }

    SECTION("rcurly success") {
        const auto result = sp_delimiter_tokenizer.tokenize("}a");
        REQUIRE(result.has_value());

        const auto& [success, rest] = result.value();
        REQUIRE(success.T == TokenType::RCurly);
        REQUIRE(success.content == "}");
        REQUIRE(rest == "a");
    }

    SECTION("delimiter failure - integer") {
        const auto result = sp_delimiter_tokenizer.tokenize("1");
        REQUIRE(!result.has_value());
    }
}
