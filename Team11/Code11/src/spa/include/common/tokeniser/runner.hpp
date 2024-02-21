#pragma once

#include "common/tokeniser/tokenizer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace tokenizer {
class TokenizerRunner {
    std::unique_ptr<Tokenizer> tokenizer;

    static void push_token(bool debug, std::vector<Token>& result, const Token& res_success);
    bool include_done;

  public:
    explicit TokenizerRunner(std::unique_ptr<Tokenizer> tokeniser, bool include_done = false)
        : tokenizer(std::move(tokeniser)), include_done(include_done){};

    [[nodiscard]] auto apply_tokeniser(std::string input, bool debug = false) const -> std::vector<Token>;
};
} // namespace tokenizer
