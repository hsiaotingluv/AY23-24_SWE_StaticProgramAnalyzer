#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-suspicious-missing-comma"
#include "catch.hpp"

#include "sp/cfg/cfg_builder.hpp"
#include "sp/main.hpp"
#include "sp/parser/program_parser.hpp"
#include "sp/tokeniser/tokeniser.hpp"
#include "sp/traverser/traverser.hpp"
#include <unordered_set>

auto get_stmt_nums_in_proc(sp::ProcMap proc_map, std::string proc_name) -> std::unordered_set<int> {
    std::unordered_set<int> all_stmt_nums;
    auto cfg = proc_map.at(proc_name);
    for (const auto& [cfg_node, outneighbours] : cfg->graph) {
        auto stmt_nums = cfg_node->stmt_nums;
        all_stmt_nums.insert(stmt_nums.begin(), stmt_nums.end());
    }
    return all_stmt_nums;
}

auto get_proc_names(sp::ProcMap proc_map) {
    std::unordered_set<std::string> proc_names;
    for (const auto& [proc_name, cfg] : proc_map) {
        proc_names.insert(proc_name);
    }
    return proc_names;
}

TEST_CASE("Test CFG Builder") {
    auto tokenizer_runner =
        std::make_shared<tokenizer::TokenizerRunner>(std::make_unique<sp::SourceProcessorTokenizer>(), true);
    auto parser = std::make_shared<sp::ProgramParser>();
    auto [read_facade, write_facade] = PKB::create_facades();
    auto cfg_builder = std::make_shared<sp::CfgBuilder>();
    std::vector<std::shared_ptr<sp::Traverser>> traversers = {std::make_shared<sp::StmtNumTraverser>(write_facade)};
    auto sp = sp::SourceProcessor{tokenizer_runner, parser, cfg_builder, traversers};

    SECTION("complex program Code 4 - success") {
        std::string input = R"(procedure main {
            flag = 0;
            call computeCentroid;
            call printResults;
        }

        procedure readPoint {
            read x;
            read y;
        }

        procedure printResults {
            print flag;
            print cenX;
            print cenY;
            print normSq;
        }

        procedure computeCentroid {
            count = 0;
            cenX = 0;
            cenY = 0;
            call readPoint;
            while ((x != 0) && (y != 0)) {
                count = count + 1;
                cenX = cenX + x;
                cenY = cenY + y;
                call readPoint;
            }
            if (count == 0) then {
                flag = 1;
            } else {
                cenX = cenX / count;
                cenY = cenY / count;
            }
            normSq = cenX * cenX + cenY * cenY;
        })";

        auto ast = sp.process(input);

        /**
         * cfg_builder->to_string() returns the below string representation of the Control Flow Graph.
         *
         * computeCentroid:
         * Node(21, 22) -> OutNeighbours(Node(23))
         * Node(23) -> OutNeighbours()
         * Node(20) -> OutNeighbours(Node(23))
         * Node(19) -> OutNeighbours(Node(20), Node(21, 22))
         * Node(15, 16, 17, 18) -> OutNeighbours(Node(14))
         * Node(14) -> OutNeighbours(Node(15, 16, 17, 18), Node(19))
         * Node(10, 11, 12, 13) -> OutNeighbours(Node(14))
         * readPoint:
         * Node(4, 5) -> OutNeighbours()
         * printResults:
         * Node(6, 7, 8, 9) -> OutNeighbours()
         * main:
         * Node(1, 2, 3) -> OutNeighbours()
         */

        REQUIRE(get_proc_names(cfg_builder->proc_map) ==
                std::unordered_set<std::string>{"main", "readPoint", "printResults", "computeCentroid"});
        REQUIRE(get_stmt_nums_in_proc(cfg_builder->proc_map, "main") == std::unordered_set<int>{1, 2, 3});
        REQUIRE(get_stmt_nums_in_proc(cfg_builder->proc_map, "readPoint") == std::unordered_set<int>{4, 5});
        REQUIRE(get_stmt_nums_in_proc(cfg_builder->proc_map, "printResults") == std::unordered_set<int>{6, 7, 8, 9});
        REQUIRE(get_stmt_nums_in_proc(cfg_builder->proc_map, "computeCentroid") ==
                std::unordered_set<int>{10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23});
    }
}

#pragma clang diagnostic pop