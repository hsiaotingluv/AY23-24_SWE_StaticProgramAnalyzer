#pragma once

#include "common/tokeniser/runner.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/annotator/annotator.hpp"
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
    std::shared_ptr<Annotator> annotator;

  public:
    SourceProcessor(TokenizerRunner& tr, std::shared_ptr<Parser> parser, std::shared_ptr<Annotator> annotator)
        : tokenizer_runner(tr), parser(std::move(parser)), annotator(std::move(annotator)) {
    }

    auto parse(std::string& input) -> std::shared_ptr<AstNode> {
        auto tokens = tokenizer_runner.apply_tokeniser(std::move(input));
        auto it = tokens.cbegin();
        auto ast = parser->parse(it, tokens.end());
        return ast;
    }
};
} // namespace sp