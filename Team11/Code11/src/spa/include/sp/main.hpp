#pragma once

#include "common/tokeniser/runner.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include <memory>
#include <utility>

using namespace tokenizer;

namespace sp {

/**
 * Note that C++23 has stacktrace support for better error reporting but this project is targeting C++17
 */
class SourceProcessor {
    TokenizerRunner& tokenizer_runner;
    std::shared_ptr<Parser> parser;

  public:
    SourceProcessor(TokenizerRunner& tr, std::shared_ptr<Parser> parser)
        : tokenizer_runner(tr), parser(std::move(parser)) {
    }

    auto parse(std::string& input) -> std::shared_ptr<AstNode> {
        auto tokens = tokenizer_runner.apply_tokeniser(std::move(input));
        auto it = tokens.cbegin();
        auto ast = parser->parse(it, tokens.end());
        return ast;
    }
};
} // namespace sp