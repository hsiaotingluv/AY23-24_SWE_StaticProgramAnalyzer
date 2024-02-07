#include "catch.hpp"
#include "common/tokeniser/runner.hpp"
#include "qps/parser/expression_parser.hpp"
#include "qps/tokeniser/tokeniser.hpp"

#include <memory>
#include <tuple>
#include <vector>

using namespace qps;

static const std::vector<std::tuple<std::string, std::string>> exprs = {
    {"42", "(42)"},
    {"42 + 42", "((42)+(42))"},
    {"v + x * y + z * t", "(((v)+((x)*(y)))+((z)*(t)))"},
    {"x + z", "((x)+(z))"},
    {" x + y + z", "(((x)+(y))+(z))"},
    {"x + z * 5", "((x)+((z)*(5)))"},
    {"z * 5 + x", "(((z)*(5))+(x))"},
    {"(x + z) * 5", "(((x)+(z))*(5))"},
    {"z + 2 *(v+ x)", "((z)+((2)*((v)+(x))))"},
    {"z + 2* (v +x)", "((z)+((2)*((v)+(x))))"}};

TEST_CASE("Test constant") {
    const auto runner = tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

    SECTION("constant success") {
        const auto query = "42";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = constant(tokens.begin(), tokens.end());

        REQUIRE(result.has_value());
        const auto& [success, rest] = result.value();
        REQUIRE(success.value == "(42)");
        REQUIRE(rest == tokens.end());

        const auto query2 = "42 + 42";
        auto tokens2 = runner.apply_tokeniser(query2);
        const auto result2 = constant(tokens2.begin(), tokens2.end());

        REQUIRE(result2.has_value());
        const auto& [success2, rest2] = result2.value();
        REQUIRE(success2.value == "(42)");
        REQUIRE(rest2 == tokens2.begin() + 1);
    }

    SECTION("constant failure - letter") {
        const auto query = "a";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = constant(tokens.begin(), tokens.end());

        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test variable") {
    const auto runner = tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};
    SECTION("variable success") {
        const auto query = "a";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = variable(tokens.begin(), tokens.end());

        REQUIRE(result.has_value());
        const auto& [success, rest] = result.value();
        REQUIRE(success.value == "(a)");
        REQUIRE(rest == tokens.end());

        const auto query2 = "a + 42";
        auto tokens2 = runner.apply_tokeniser(query2);
        const auto result2 = variable(tokens2.begin(), tokens2.end());

        REQUIRE(result2.has_value());
        const auto& [success2, rest2] = result2.value();
        REQUIRE(success2.value == "(a)");
        REQUIRE(rest2 == tokens2.begin() + 1);
    }

    SECTION("variable failure - integer") {
        const auto query = "42";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = variable(tokens.begin(), tokens.end());

        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Test Expression") {
    const auto runner = tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};
    SECTION("expression success - constant") {
        const auto query = "42";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = expression(tokens.begin(), tokens.end());

        REQUIRE(result.has_value());
        const auto& [success, rest] = result.value();
        REQUIRE(success.value == "(42)");
        REQUIRE(rest == tokens.end());
    }

    SECTION("expression success - variable") {
        const auto query = "a";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = expression(tokens.begin(), tokens.end());

        REQUIRE(result.has_value());
        const auto& [success, rest] = result.value();
        REQUIRE(success.value == "(a)");
        REQUIRE(rest == tokens.end());
    }

    SECTION("expression success - binary operation") {
        const auto query = "42 + a";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = expression(tokens.begin(), tokens.end());

        REQUIRE(result.has_value());
        const auto& [success, rest] = result.value();
        REQUIRE(success.value == "((42)+(a))");
        REQUIRE(rest == tokens.end());
    }

    SECTION("expression success - binary operation with parentheses") {
        const auto query = "(42 + a)";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = expression(tokens.begin(), tokens.end());

        REQUIRE(result.has_value());
        const auto& [success, rest] = result.value();
        REQUIRE(success.value == "((42)+(a))");
        REQUIRE(rest == tokens.end());
    }

    SECTION("expression failure - invalid binary operation") {
        const auto query = "42 +";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = expression(tokens.begin(), tokens.end());

        REQUIRE(!result.has_value());
    }

    SECTION("expression failure - invalid binary operation with parentheses") {
        const auto query = "(42 +";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = expression(tokens.begin(), tokens.end());

        REQUIRE(!result.has_value());
    }

    SECTION("expression success - operator precedence") {
        const auto query = "42 + 42 * 42";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = expression(tokens.begin(), tokens.end());

        REQUIRE(result.has_value());
        const auto& [success, rest] = result.value();
        REQUIRE(success.value == "((42)+((42)*(42)))");
        REQUIRE(rest == tokens.end());

        const auto query2 = "v + x * y + z * t";
        auto tokens2 = runner.apply_tokeniser(query2);
        const auto result2 = expression(tokens2.begin(), tokens2.end());

        REQUIRE(result2.has_value());
        const auto& [success2, rest2] = result2.value();
        REQUIRE(success2.value == "(((v)+((x)*(y)))+((z)*(t)))");
        REQUIRE(rest2 == tokens2.end());

        const auto query3 = "z + 2 * (v + x)";
        auto tokens3 = runner.apply_tokeniser(query3);
        const auto result3 = expression(tokens3.begin(), tokens3.end());

        REQUIRE(result3.has_value());
        const auto& [success3, rest3] = result3.value();
        REQUIRE(success3.value == "((z)+((2)*((v)+(x))))");
        REQUIRE(rest3 == tokens3.end());
    }

    SECTION("expression success - operator precedence with parentheses") {
        for (const auto& [query, expected] : exprs) {
            auto tokens = runner.apply_tokeniser(query);
            const auto result = expression(tokens.begin(), tokens.end());

            REQUIRE(result.has_value());
            const auto& [success, rest] = result.value();
            REQUIRE(success.value == expected);
            REQUIRE(rest == tokens.end());
        }
    }
}

TEST_CASE("Text Expression Spec") {
    const auto runner = tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

    SECTION("expression spec success - wildcard") {
        const auto query = "_";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = parse_expression_spec(tokens.begin(), tokens.end());

        REQUIRE(result.has_value());
        const auto& [success, rest] = result.value();
        REQUIRE(success.value == "_");
        REQUIRE(rest == tokens.end());
    }

    SECTION("expression spec success - quoted expression") {
        for (const auto& [query, expected] : exprs) {
            const auto query2 = "\"" + query + "\"";
            auto tokens2 = runner.apply_tokeniser(query2);
            const auto result2 = parse_expression_spec(tokens2.begin(), tokens2.end());

            REQUIRE(result2.has_value());
            const auto& [success2, rest2] = result2.value();
            REQUIRE(success2.value == expected);
            REQUIRE(rest2 == tokens2.end());
        }
    }

    SECTION("expression spec success - quoted expression with wildcard") {
        for (const auto& [query, expected] : exprs) {
            const auto query2 = "_ \"" + query + "\" _";
            auto tokens2 = runner.apply_tokeniser(query2);
            const auto result2 = parse_expression_spec(tokens2.begin(), tokens2.end());

            REQUIRE(result2.has_value());
            const auto& [success2, rest2] = result2.value();
            REQUIRE(success2.value == "_" + expected + "_");
            REQUIRE(rest2 == tokens2.end());
        }
    }

    SECTION("expression spec failure - invalid expression") {
        const auto query = "42 +";
        auto tokens = runner.apply_tokeniser(query);
        const auto result = parse_expression_spec(tokens.begin(), tokens.end());

        REQUIRE(!result.has_value());
    }
}