#pragma once

#include "common/tokeniser/runner.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/parser/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/const_populator_traverser.hpp"
#include "sp/traverser/proc_populator_traverser.hpp"
#include "sp/traverser/stmt_num_traverser.hpp"
#include "sp/traverser/traverser.hpp"
#include "sp/traverser/variable_populator_traverser.hpp"
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
    std::vector<std::shared_ptr<Traverser>> traversers;

  public:
    SourceProcessor(TokenizerRunner& tr, std::shared_ptr<Parser> parser,
                    std::vector<std::shared_ptr<Traverser>>& traversers)
        : tokenizer_runner(tr), parser(std::move(parser)), traversers(std::move(traversers)) {
    }

    static auto get_complete_sp(std::shared_ptr<WriteFacade> write_facade) -> SourceProcessor {
        auto tokenizer_runner = tokenizer::TokenizerRunner{std::make_unique<SourceProcessorTokenizer>(), true};
        auto parser = std::make_shared<ProgramParser>();
        std::vector<std::shared_ptr<Traverser>> traversers = {
            std::make_shared<StmtNumTraverser>(),
            std::make_shared<ConstPopulatorTraverser>(write_facade),
            std::make_shared<VarPopulatorTraverser>(write_facade),
            std::make_shared<ProcedurePopulatorTraverser>(write_facade),
        };

        return SourceProcessor{tokenizer_runner, parser, traversers};
    }

    auto process(std::string& input) -> std::shared_ptr<AstNode> {
        auto tokens = tokenizer_runner.apply_tokeniser(std::move(input));
        auto it = tokens.cbegin();
        auto ast = parser->parse(it, tokens.end());
        for (const auto& traverser : traversers) {
            ast = traverser->traverse(ast);
        }

        return ast;
    }
};
} // namespace sp