#pragma once

#include "common/tokeniser/tokenizer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace tokenizer {
class TokenizerRunner {
    const std::unique_ptr<Tokenizer> tokenizer;

    static void push_token(bool debug, std::vector<Token>& result, const Token& res_success);

  public:
    TokenizerRunner(std::unique_ptr<Tokenizer> tokeniser) : tokenizer(std::move(tokeniser)){};

    [[nodiscard]] auto apply_tokeniser(std::string input, bool debug = false) const -> std::vector<Token>;
};
} // namespace tokenizer
