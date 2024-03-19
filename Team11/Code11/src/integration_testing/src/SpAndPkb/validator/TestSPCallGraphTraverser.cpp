#include "catch.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/pkb_manager.h"
#include "sp/main.hpp"
#include "sp/validator/call_graph_traverser.hpp"

using namespace sp;
using ProcNames = std::unordered_set<std::string>;

/**
 * @brief Traverse the CallGraph while resetting the Facades for testing purposes.
 */
auto traverse(const sp::SemanticValidator::CallGraph& call_graph) -> std::shared_ptr<pkb::ReadFacade> {
    auto [read_facade, write_facade] = pkb::PkbManager::create_facades(); // Wipe cache
    auto call_graph_traverser = std::make_shared<CallGraphTraverser>(write_facade);
    call_graph_traverser->traverse(call_graph);
    return read_facade;
}

TEST_CASE("Test Call Graph Traverser - Basic SPA") {
    std::string input = R"(
        procedure main {
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

    auto [read_facade, write_facade] = pkb::PkbManager::create_facades();
    auto sp = sp::SourceProcessor::get_complete_sp(write_facade);
    auto ast = sp->process(input);

    SECTION("Test SP and PKB Populate calls - success") {
        REQUIRE(read_facade->get_all_calls_callers().size() == 2);
        REQUIRE(read_facade->get_all_calls_callees().size() == 3);
        REQUIRE(read_facade->get_callees("main").size() == 2);
        REQUIRE(read_facade->get_callers("main").empty());
        REQUIRE(read_facade->get_callers("computeCentroid").size() == 1);
    }
}

TEST_CASE("Test SP Call Graph Traverser - Simple Usability Tests") {
    const sp::SemanticValidator::CallGraph EMPTY_CALLGRAPH = {};
    const sp::SemanticValidator::CallGraph ONE_TO_NONE_CALLGRAPH = {{"Procedure1", ProcNames()},
                                                                    {"Procedure2", ProcNames()}};
    const sp::SemanticValidator::CallGraph ONE_TO_ONE_CALLGRAPH = {{"Procedure1", ProcNames({"Procedure2"})},
                                                                   {"Procedure2", ProcNames({"Procedure3"})}};
    const sp::SemanticValidator::CallGraph ONE_TO_MANY_CALLGRAPH = {
        {"Procedure1", ProcNames({"Procedure2", "Procedure3", "Procedure4"})},
        {"Procedure2", ProcNames({"Procedure3", "Procedure4", "Procedure5"})},
        {"Procedure3", ProcNames({"Procedure4", "Procedure5", "Procedure6"})}};

    SECTION("Test Traversing an Empty CallGraph - Success") {
        auto read_facade = traverse(EMPTY_CALLGRAPH);
        REQUIRE(read_facade->get_all_calls_callees() == ProcNames());
        REQUIRE(read_facade->get_all_calls_callers() == ProcNames());
    }

    SECTION("Test Traversing a CallGraph with No Caller - Success") {
        auto read_facade = traverse(ONE_TO_NONE_CALLGRAPH);
        REQUIRE(read_facade->get_all_calls_callees() == ProcNames()); // Nothing to write home about.
        REQUIRE(read_facade->get_all_calls_callers() == ProcNames());
    }

    SECTION("Test Traversing a CallGraph with 1 Caller - Success") {
        auto read_facade = traverse(ONE_TO_ONE_CALLGRAPH);
        REQUIRE(read_facade->get_all_calls_callees() == ProcNames({"Procedure1", "Procedure2"}));
        REQUIRE(read_facade->get_all_calls_callers() == ProcNames({"Procedure2", "Procedure3"}));
        REQUIRE(read_facade->has_calls_relation("Procedure2", "Procedure1"));
        REQUIRE(read_facade->has_calls_relation("Procedure3", "Procedure2"));
    }

    SECTION("Test Traversing a CallGraph with Multiple Callers - Success") {
        auto read_facade = traverse(ONE_TO_MANY_CALLGRAPH);
        REQUIRE(read_facade->get_all_calls_callees() == ProcNames({"Procedure1", "Procedure2", "Procedure3"}));
        REQUIRE(read_facade->get_all_calls_callers() ==
                ProcNames({"Procedure2", "Procedure3", "Procedure4", "Procedure5", "Procedure6"}));
        REQUIRE(read_facade->has_calls_relation("Procedure2", "Procedure1"));
        REQUIRE(read_facade->has_calls_relation("Procedure3", "Procedure1"));
        REQUIRE(read_facade->has_calls_relation("Procedure4", "Procedure1"));
        REQUIRE(read_facade->has_calls_relation("Procedure3", "Procedure2"));
        REQUIRE(read_facade->has_calls_relation("Procedure4", "Procedure2"));
        REQUIRE(read_facade->has_calls_relation("Procedure5", "Procedure2"));
        REQUIRE(read_facade->has_calls_relation("Procedure4", "Procedure3"));
        REQUIRE(read_facade->has_calls_relation("Procedure5", "Procedure3"));
        REQUIRE(read_facade->has_calls_relation("Procedure6", "Procedure3"));
    }
}