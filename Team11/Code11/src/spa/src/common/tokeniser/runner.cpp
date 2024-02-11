#include "common/tokeniser/runner.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace tokenizer;

auto TokenizerRunner::apply_tokeniser(std::string input, bool debug) const -> std::vector<Token> {
    std::vector<Token> result{};

    while (!input.empty()) {
        const auto res = tokenizer->tokenize(input);
        if (!res.has_value()) {
            std::stringstream error_message;
            error_message << "Tokeniser error!\n"
                          << "Found unexpected token: " << input.substr(0, input.find("\n")) << std::endl;
            throw std::runtime_error(error_message.str());
        } else {
            const auto& [res_success, rest] = res.value();
            input = rest;
            if (res_success.T != TokenType::Junk) {
                push_token(debug, result, res_success);
            }
        }
    }

    if (include_done) {
        auto done_token = Token{TokenType::Done, ""};
        result.push_back(done_token);
    }
    return result;
}

void TokenizerRunner::push_token(bool debug, std::vector<Token>& result, const Token& res_success) {
    result.push_back(res_success);
    if (debug) {
        std::cout << "Found: " << res_success << std::endl;
    }
}
