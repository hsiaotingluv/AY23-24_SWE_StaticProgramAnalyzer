#pragma once

#include "common/tokeniser/runner.hpp"

#include "common/ast/ast.hpp"
#include "sp/cfg/cfg_builder.hpp"
#include "sp/parser/parser.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/design_entites_populator_traverser.hpp"
#include "sp/traverser/follows_traverser.hpp"
#include "sp/traverser/modifies_traverser.hpp"
#include "sp/traverser/next_traverser.hpp"
#include "sp/traverser/parent_traverser.hpp"
#include "sp/traverser/stmt_num_traverser.hpp"
#include "sp/traverser/traverser.hpp"
#include "sp/traverser/uses_traverser.hpp"
#include "sp/validator/call_graph_traverser.hpp"
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
    std::shared_ptr<StmtNumTraverser> stmt_num_traverser;
    std::shared_ptr<CfgBuilder> cfg_builder;
    std::vector<std::shared_ptr<Traverser>> design_abstr_traversers;
    std::shared_ptr<NextTraverser> next_traverser;
    std::shared_ptr<pkb::WriteFacade> write_facade;
    SemanticValidator semantic_validator{};
    CallGraphTraverser call_graph_traverser;

  public:
    SourceProcessor(std::shared_ptr<TokenizerRunner> tr, std::shared_ptr<Parser> parser,
                    std::shared_ptr<StmtNumTraverser> stmt_num_traverser, std::shared_ptr<CfgBuilder> cfg_builder,
                    const std::vector<std::shared_ptr<Traverser>>& traversers,
                    const std::shared_ptr<NextTraverser> next_traverser)
        : tokenizer_runner(std::move(tr)), parser(std::move(parser)), stmt_num_traverser(std::move(stmt_num_traverser)),
          cfg_builder(std::move(cfg_builder)), design_abstr_traversers(traversers),
          next_traverser(std::move(next_traverser)) {
    }

    SourceProcessor(std::shared_ptr<TokenizerRunner> tr, std::shared_ptr<Parser> parser,
                    std::shared_ptr<StmtNumTraverser> stmt_num_traverser, std::shared_ptr<CfgBuilder> cfg_builder,
                    const std::vector<std::shared_ptr<Traverser>>&& traversers,
                    const std::shared_ptr<NextTraverser> next_traverser,
                    const std::shared_ptr<pkb::WriteFacade>& write_facade)
        : tokenizer_runner(std::move(tr)), parser(std::move(parser)), stmt_num_traverser(std::move(stmt_num_traverser)),
          cfg_builder(std::move(cfg_builder)), design_abstr_traversers(traversers),
          next_traverser(std::move(next_traverser)), write_facade(write_facade), call_graph_traverser(write_facade) {
    }

    static auto get_complete_sp(const std::shared_ptr<pkb::WriteFacade>& write_facade)
        -> std::shared_ptr<SourceProcessor> {
        return std::make_shared<SourceProcessor>(
            std::make_shared<tokenizer::TokenizerRunner>(std::make_unique<SourceProcessorTokenizer>(), true),
            std::make_shared<ProgramParser>(), std::make_shared<StmtNumTraverser>(write_facade),
            std::make_shared<CfgBuilder>(),
            std::vector<std::shared_ptr<Traverser>>{
                std::make_shared<DesignEntitiesPopulatorTraverser>(write_facade),
                std::make_shared<ModifiesTraverser>(write_facade), std::make_shared<ParentTraverser>(write_facade),
                std::make_shared<UsesTraverser>(write_facade), std::make_shared<FollowsTraverser>(write_facade)},
            std::make_shared<NextTraverser>(write_facade), write_facade);
    }

    static auto output_xml(const std::shared_ptr<AstNode>& ast_node) -> std::string {
        static int counter = 0;
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

        // Step 1. Tokenise SIMPLE
        const auto tokens = tokenizer_runner->apply_tokeniser(std::move(input));

        // Step 2. Parse tokens
        auto it = tokens.begin();
        auto ast = parser->parse(it, tokens.end());

        // Step 3. Traverse AST
        // Step 3.1 Get Procedure Topology Order
        auto procedure_topology_orders = semantic_validator.validate_get_traversal_order(ast);
        // Step 3.2 Execute Statement Number Traverser （Update AST)
        ast = stmt_num_traverser->traverse(ast, procedure_topology_orders);
        // Step 3.3 Build CFG
        auto cfgs = cfg_builder->build(ast);
        // Step 3.4 Extract Call Graph
        call_graph_traverser.traverse(semantic_validator.get_call_graph());

        // Step 4. Execute Design Abstraction Traversers (Update AST)
        for (const auto& traverser : design_abstr_traversers) {
            ast = traverser->traverse(ast, procedure_topology_orders);
        }

        // Step 5. Execute Next Traverser
        next_traverser->traverse(cfgs);

        // Step 6. If writing to pkb, finalise pkb.
        if (write_facade != nullptr) {
            write_facade->finalise_pkb();
        }

#ifdef DEBUG
        output_xml(ast);
#endif
        return ast;
    }
};
} // namespace sp