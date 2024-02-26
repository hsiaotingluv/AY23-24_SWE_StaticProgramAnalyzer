#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "sp/tokeniser/tokeniser.hpp"

using namespace tokenizer;

TEST_CASE("Test SP Tokenizer") {
    const auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<sp::SourceProcessorTokenizer>()};

    SECTION("Complex Query - success") {
        const auto query = R"(procedure computeAverage {
                read num1;
                read num2;

                sum = num1 + num2;
                print sum;
            }
        )";

        const auto result = tokenizer_runner.apply_tokeniser(query);

        auto answers = std::vector<Token>{
            Token{TokenType::String, "procedure"},
            Token{TokenType::String, "computeAverage"},
            Token{TokenType::LCurly, "{"},
            Token{TokenType::String, "read"},
            Token{TokenType::String, "num1"},
            Token{TokenType::Semicolon, ";"},
            Token{TokenType::String, "read"},
            Token{TokenType::String, "num2"},
            Token{TokenType::Semicolon, ";"},
            Token{TokenType::String, "sum"},
            Token{TokenType::Assignment, "="},
            Token{TokenType::String, "num1"},
            Token{TokenType::Add, "+"},
            Token{TokenType::String, "num2"},
            Token{TokenType::Semicolon, ";"},
            Token{TokenType::String, "print"},
            Token{TokenType::String, "sum"},
            Token{TokenType::Semicolon, ";"},
            Token{TokenType::RCurly, "}"},
        };

        REQUIRE(result.size() == 19);
        for (int i = 0; i < result.size(); i++) {
            REQUIRE(result[i].content == answers[i].content);
            REQUIRE(result[i].T == answers[i].T);
        }
    }

    SECTION("simple relational - success") {
        const auto query = R"(x != 2)";

        const auto result = tokenizer_runner.apply_tokeniser(query);
        REQUIRE(result[0].T == TokenType::String);
        REQUIRE(result[1].T == TokenType::NotEqual);
        REQUIRE(result[2].T == TokenType::Integer);
    }

    SECTION("Complex Query Unknown tokens - fails") {
        const auto query = R"(procedure computeAverage {
                read num1^;
                read num2;

                sum = num1 | num2;
                print sum;
            }
        )";

        REQUIRE_THROWS(tokenizer_runner.apply_tokeniser(query));
    }
}
