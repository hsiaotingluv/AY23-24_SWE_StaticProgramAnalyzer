#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "qps/tokeniser/tokeniser.hpp"
#include "qps/tokeniser/wildcard_tokeniser.hpp"

#include <memory>

using namespace tokenizer;

TEST_CASE("Test WildCard Tokenizer") {
    const auto tokenizer = std::make_unique<qps::WildCardTokenizer>();

    SECTION("Wildcard - success") {
        const auto query = R"(_X)";
        const auto maybe_success = tokenizer->tokenize(query);

        REQUIRE(maybe_success.has_value());
        const auto& [token, rest] = maybe_success.value();
        REQUIRE(token.content == "_");
        REQUIRE(token.T == TokenType::Wildcard);

        REQUIRE(rest == "X");
    }

    SECTION("Wildcard - failure") {
        const auto query = R"(a)";
        const auto maybe_success = tokenizer->tokenize(query);

        REQUIRE(!maybe_success.has_value());
    }
}

TEST_CASE("Test QPS Tokenizer") {
    const auto tokenizer_runner = TokenizerRunner(std::make_unique<qps::QueryProcessingSystemTokenizer>());

    SECTION("Complex Query - success") {
        const auto query = R"(assign a; Select a + y such that Modifies(_, "b123 * 456"))";
        const auto result = tokenizer_runner.apply_tokeniser(query);

        REQUIRE(result.size() == 19);
        REQUIRE(result[0].content == "assign");
        REQUIRE(result[0].T == TokenType::String);

        REQUIRE(result[1].content == "a");
        REQUIRE(result[1].T == TokenType::String);

        REQUIRE(result[2].content == ";");
        REQUIRE(result[2].T == TokenType::Semicolon);

        REQUIRE(result[3].content == "Select");
        REQUIRE(result[3].T == TokenType::String);

        REQUIRE(result[4].content == "a");
        REQUIRE(result[4].T == TokenType::String);

        REQUIRE(result[5].content == "+");
        REQUIRE(result[5].T == TokenType::Add);

        REQUIRE(result[6].content == "y");
        REQUIRE(result[6].T == TokenType::String);

        REQUIRE(result[7].content == "such");
        REQUIRE(result[7].T == TokenType::String);

        REQUIRE(result[8].content == "that");
        REQUIRE(result[8].T == TokenType::String);

        REQUIRE(result[9].content == "Modifies");
        REQUIRE(result[9].T == TokenType::String);

        REQUIRE(result[10].content == "(");
        REQUIRE(result[10].T == TokenType::LParen);

        REQUIRE(result[11].content == "_");
        REQUIRE(result[11].T == TokenType::Wildcard);

        REQUIRE(result[12].content == ",");
        REQUIRE(result[12].T == TokenType::Comma);

        REQUIRE(result[13].content == "\"");
        REQUIRE(result[13].T == TokenType::DQuote);

        REQUIRE(result[14].content == "b123");
        REQUIRE(result[14].T == TokenType::String);

        REQUIRE(result[15].content == "*");
        REQUIRE(result[15].T == TokenType::Mul);

        REQUIRE(result[16].content == "456");
        REQUIRE(result[16].T == TokenType::Integer);

        REQUIRE(result[17].content == "\"");
        REQUIRE(result[17].T == TokenType::DQuote);

        REQUIRE(result[18].content == ")");
        REQUIRE(result[18].T == TokenType::RParen);
    }
}
