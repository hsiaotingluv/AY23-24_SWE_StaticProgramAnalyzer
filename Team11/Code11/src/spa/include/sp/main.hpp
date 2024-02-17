#pragma once

#include "common/tokeniser/runner.hpp"

#include "common/ast/ast.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/design_entites_populator_traverser.hpp"
#include "sp/traverser/modifies_traverser.hpp"
#include "sp/traverser/stmt_num_traverser.hpp"
#include "sp/traverser/traverser.hpp"
#include "sp/validator/semantic_validator.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <utility>
#include <vector>

using namespace tokenizer;

namespace sp {

/**
 * Note that C++23 has stacktrace support for better error reporting but this project is targeting C++17
 */
class SourceProcessor {
    std::shared_ptr<TokenizerRunner> tokenizer_runner;
    std::shared_ptr<Parser> parser;
    std::vector<std::shared_ptr<Traverser>> traversers;
    SemanticValidator semantic_validator{};
    static int counter;

  public:
    SourceProcessor(std::shared_ptr<TokenizerRunner> tr, std::shared_ptr<Parser> parser,
                    const std::vector<std::shared_ptr<Traverser>>& traversers)
        : tokenizer_runner(std::move(tr)), parser(std::move(parser)), traversers(traversers) {
    }

    SourceProcessor(std::shared_ptr<TokenizerRunner>&& tr, std::shared_ptr<Parser>&& parser,
                    std::vector<std::shared_ptr<Traverser>>&& traversers)
        : tokenizer_runner(std::move(tr)), parser(std::move(parser)), traversers(std::move(traversers)) {
    }

    static auto get_complete_sp(const std::shared_ptr<WriteFacade>& write_facade) -> std::shared_ptr<SourceProcessor> {
        return std::make_shared<SourceProcessor>(
            std::make_shared<tokenizer::TokenizerRunner>(std::make_unique<SourceProcessorTokenizer>(), true),
            std::make_shared<ProgramParser>(),
            std::vector<std::shared_ptr<Traverser>>{
                std::make_shared<StmtNumTraverser>(write_facade),
                std::make_shared<DesignEntitiesPopulatorTraverser>(write_facade),
                std::make_shared<ModifiesTraverser>(write_facade),
            });
    }

    static auto output_xml(const std::shared_ptr<AstNode>& ast_node) -> std::string {
        auto ast_xml = ast_node->to_xml();
        auto current_path = std::filesystem::current_path();

        while (!current_path.empty() && current_path.filename() != "23s2-cp-spa-team-11") {
            current_path = current_path.parent_path();
        }

        auto default_folder = std::string(current_path.string() + "/Team11/Code11/tests/");
        auto filename = default_folder + "source_xml_" + std::to_string(counter++) + ".xml";
        std::ofstream output_file(filename);

        if (!output_file.is_open()) {
            throw std::runtime_error("Unable to write file to output xml " + filename);
        }

        output_file << ast_xml;
        output_file.close();

        return ast_xml;
    }

    auto process(std::string& input) -> std::shared_ptr<AstNode> {
        const auto tokens = tokenizer_runner->apply_tokeniser(std::move(input));
        auto it = tokens.begin();
        auto ast = parser->parse(it, tokens.end());
        auto procedure_topology_orders = semantic_validator.validate_get_traversal_order(ast);

        for (const auto& traverser : traversers) {
            ast = traverser->traverse(ast, procedure_topology_orders);
        }

#ifdef DEBUG
        output_xml(ast);
#endif
        return ast;
    }
};
} // namespace sp