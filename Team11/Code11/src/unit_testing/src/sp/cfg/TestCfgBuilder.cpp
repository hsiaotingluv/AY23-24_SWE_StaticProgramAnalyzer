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

auto get_proc_names(sp::ProcMap proc_map) -> std::unordered_set<std::string> {
    std::unordered_set<std::string> proc_names;
    for (const auto& [proc_name, cfg] : proc_map) {
        proc_names.insert(proc_name);
    }
    return proc_names;
}

auto get_dummy_nodes(sp::ProcMap proc_map) -> int {
    int num_dummy = 0;
    for (const auto& [proc_name, cfg] : proc_map) {
        for (const auto& [cfg_node, outneighbours] : cfg->graph) {
            if (cfg_node->stmt_nums.empty()) {
                num_dummy++;
            }
        }
    }
    return num_dummy;
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
         * FYI : cfg_builder->to_string() returns the below string representation of the Control Flow Graph.
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
        REQUIRE(get_dummy_nodes(cfg_builder->proc_map) == 0);
    }

    SECTION("Single Procedure ending with If Statement - success") {
        std::string input = R"(procedure computeCentroid {
            while ((x != 0) && (y != 0)) {
                count = count + 1;
                cenX = cenX + x;
                cenY = cenY + y;
            }
            if (count == 0) then {
                flag = 1;
            } else {
                cenX = cenX / count;
                cenY = cenY / count;
            }
        })";

        auto ast = sp.process(input);

        /**
         * FYI : cfg_builder->to_string() returns the below string representation of the Control Flow Graph.
         *
         * computeCentroid:
         * Node(7, 8) -> OutNeighbours(Node()) // Points to dummy node
         * Node() -> OutNeighbours() // Dummy node
         * Node(6) -> OutNeighbours(Node()) // Points to dummy node
         * Node(5) -> OutNeighbours(Node(6), Node(7, 8))
         * Node(2, 3, 4) -> OutNeighbours(Node(1))
         * Node(1) -> OutNeighbours(Node(2, 3, 4), Node(5))
         */

        REQUIRE(get_proc_names(cfg_builder->proc_map) == std::unordered_set<std::string>{"computeCentroid"});
        REQUIRE(get_stmt_nums_in_proc(cfg_builder->proc_map, "computeCentroid") ==
                std::unordered_set<int>{1, 2, 3, 4, 5, 6, 7, 8});
        // Write evaluation for if-else statements.
        REQUIRE(get_dummy_nodes(cfg_builder->proc_map) == 1);
    }

    SECTION("Multiple If Statements ending with Elses - success") {
        // There are multiple dummy nodes. For clarity of implementation, they will NOT be merged together.
        std::string input = R"(
            procedure nesting {
                if (i>j) then {
                    read i;
                } else {
                    if (i==j) then {
                        if (j < 0) then {
                            print i;
                        } else {
                            print j;}
                    } else {
                            read j;}}
            }
        )";

        /**
         * cfg_builder->to_string() returns the below string representation of the Control Flow Graph.
         *
         * computeCentroid:
         * Node(7) -> OutNeighbours(Node()) // read j
         * Node() -> OutNeighbours(Node()) // #3 dummy node  for if(i==j). Points to #1 dummy node.
         * Node(6) -> OutNeighbours(Node()) // print j
         * Node() -> OutNeighbours(Node()) // #2 dummy node for if(j<0). Points to #3 dummy node.
         * Node(5) -> OutNeighbours(Node()) // print i
         * Node(4) -> OutNeighbours(Node(5), Node(6)) // if(j<0)
         * Node(3) -> OutNeighbours(Node(4), Node(7)) // if(i==j)
         * Node() -> OutNeighbours() // #1 dummy node for if (i>j). Points to nothing. It's the end.
         * Node(2) -> OutNeighbours(Node()) // read i;
         * Node(1) -> OutNeighbours(Node(2), Node(3)) // if(i>j)
         */

        auto ast = sp.process(input);
        //std::cout << cfg_builder->to_string() << std::endl;  // To inspect the string output of the CFG.
        REQUIRE(get_proc_names(cfg_builder->proc_map) == std::unordered_set<std::string>{"nesting"});
        REQUIRE(get_stmt_nums_in_proc(cfg_builder->proc_map, "nesting") ==
                std::unordered_set<int>{1,2,3,4,5,6,7});
        REQUIRE(get_dummy_nodes(cfg_builder->proc_map) == 3);

    }
}

#pragma clang diagnostic pop