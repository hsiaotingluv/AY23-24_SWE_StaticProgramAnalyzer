#include "qps/utils/algo.h"
#include <algorithm>
#include <catch.hpp>

TEST_CASE("Algo tests") {
    SECTION("Test create permutations with 1 set") {
        std::unordered_set<std::string> set = {"a", "b", "c"};
        auto result = create_permutations(set);

        REQUIRE(result.size() == 9);
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "a")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "b")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "c")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("b", "a")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("b", "b")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("b", "c")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("c", "a")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("c", "b")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("c", "c")) !=
                result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("d", "d")) !=
                      result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "d")) !=
                      result.end());
    }

    SECTION("Test create permutations with 2 sets") {
        std::unordered_set<std::string> set1 = {"a", "b", "c"};
        std::unordered_set<std::string> set2 = {"x", "y", "z"};
        auto result = create_permutations(set1, set2);

        REQUIRE(result.size() == 9);
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "x")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "y")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "z")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("b", "x")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("b", "y")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("b", "z")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("c", "x")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("c", "y")) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("c", "z")) !=
                result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("d", "d")) !=
                      result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "d")) !=
                      result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "a")) !=
                      result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("a", "b")) !=
                      result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("x", "x")) !=
                      result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::make_pair<std::string, std::string>("x", "y")) !=
                      result.end());
    }

    SECTION("Test Tarjan SCC 1") {
        std::unordered_map<std::string, std::unordered_set<std::string>> adj_list = {
            {"a", {"b"}}, {"b", {"c"}}, {"c", {"a"}}};

        auto result = tarjan_scc(adj_list);

        REQUIRE(result.size() == 1);
        REQUIRE(result[0].size() == 3);
        REQUIRE(result[0].find("a") != result[0].end());
        REQUIRE(result[0].find("b") != result[0].end());
        REQUIRE(result[0].find("c") != result[0].end());
    }

    SECTION("Test Tarjan SCC 2") {
        std::unordered_map<std::string, std::unordered_set<std::string>> adj_list = {
            {"a", {"b", "d"}}, {"b", {"c"}}, {"c", {"a"}}, {"d", {"a"}}};

        auto result = tarjan_scc(adj_list);

        REQUIRE(result.size() == 1);
        REQUIRE(result[0].size() == 4);
        REQUIRE(result[0].find("a") != result[0].end());
        REQUIRE(result[0].find("b") != result[0].end());
        REQUIRE(result[0].find("c") != result[0].end());
        REQUIRE(result[0].find("d") != result[0].end());
    }

    SECTION("Test Tarjan SCC 3") {
        std::unordered_map<std::string, std::unordered_set<std::string>> adj_list = {
            {"a", {"b", "d"}}, {"b", {"c"}}, {"c", {"a"}}, {"d", {"a", "e"}}};

        auto result = tarjan_scc(adj_list);

        REQUIRE(result.size() == 2);
        REQUIRE(std::find(result.begin(), result.end(), std::unordered_set<std::string>{"a", "b", "c", "d"}) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::unordered_set<std::string>{"e"}) != result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::unordered_set<std::string>{"a", "e"}) !=
                      result.end());
    }

    SECTION("Test Tarjan SCC 4") {
        std::unordered_map<std::string, std::unordered_set<std::string>> adj_list = {
            {"a", {"b", "d"}}, {"b", {"c"}}, {"c", {"a"}}, {"d", {"b", "e"}}};

        auto result = tarjan_scc(adj_list);

        REQUIRE(result.size() == 2);
        REQUIRE(std::find(result.begin(), result.end(), std::unordered_set<std::string>{"a", "b", "c", "d"}) !=
                result.end());
        REQUIRE(std::find(result.begin(), result.end(), std::unordered_set<std::string>{"e"}) != result.end());
        REQUIRE_FALSE(std::find(result.begin(), result.end(), std::unordered_set<std::string>{"a", "e"}) !=
                      result.end());
    }

    SECTION("Test generate maps") {
        std::vector<std::unordered_set<std::string>> scc = {{"a", "b", "c"}, {"d", "e"}};
        auto [index_map, reverse_index_map] = generate_maps(scc);

        REQUIRE(index_map.size() == 2);
        REQUIRE(index_map[0].size() == 3);
        REQUIRE(index_map[1].size() == 2);
        REQUIRE(index_map[0].find("a") != index_map[0].end());
        REQUIRE(index_map[0].find("b") != index_map[0].end());
        REQUIRE(index_map[0].find("c") != index_map[0].end());
        REQUIRE(index_map[1].find("d") != index_map[1].end());
        REQUIRE(index_map[1].find("e") != index_map[1].end());

        REQUIRE(reverse_index_map.size() == 5);
        REQUIRE(reverse_index_map["a"] == 0);
        REQUIRE(reverse_index_map["b"] == 0);
        REQUIRE(reverse_index_map["c"] == 0);
        REQUIRE(reverse_index_map["d"] == 1);
        REQUIRE(reverse_index_map["e"] == 1);
    }

    SECTION("Test generate adj list") {
        std::unordered_map<std::string, std::unordered_set<std::string>> adj_list = {
            {"a", {"b", "d"}}, {"b", {"c"}}, {"c", {"a"}}, {"d", {"a", "e"}}};
        std::unordered_map<std::string, int> reverse_index_map = {{"a", 0}, {"b", 0}, {"c", 0}, {"d", 0}, {"e", 1}};

        auto result = generate_adj_list(adj_list, reverse_index_map);

        REQUIRE(result.size() == 1);
        REQUIRE(result.at(0) == std::unordered_set<int>{1});
    }

    SECTION("Test topo sort") {
        std::unordered_map<int, std::unordered_set<int>> adj_list = {{0, {1, 2}}, {1, {3, 6}}, {2, {3}}, {3, {4, 5}}};
        auto result = toposort(adj_list);

        REQUIRE(result.size() == 7);
        REQUIRE(std::find(result.begin(), result.end(), 5) < std::find(result.begin(), result.end(), 0));
        REQUIRE(std::find(result.begin(), result.end(), 4) < std::find(result.begin(), result.end(), 0));
        REQUIRE(std::find(result.begin(), result.end(), 3) < std::find(result.begin(), result.end(), 0));
        REQUIRE(std::find(result.begin(), result.end(), 6) < std::find(result.begin(), result.end(), 1));
        REQUIRE(std::find(result.begin(), result.end(), 1) < std::find(result.begin(), result.end(), 0));
        REQUIRE(std::find(result.begin(), result.end(), 2) < std::find(result.begin(), result.end(), 0));
        REQUIRE(std::find(result.begin(), result.end(), 5) < std::find(result.begin(), result.end(), 3));
        REQUIRE(std::find(result.begin(), result.end(), 4) < std::find(result.begin(), result.end(), 3));
        REQUIRE(std::find(result.begin(), result.end(), 4) < std::find(result.begin(), result.end(), 2));
    }

    SECTION("The whole shabang!") {
        /**
         * Ugly but hopefully understandable
         *
         * 0 -> 1 -> 2
         * |____|____^
         * v    |
         * 4    ---> 3
         *
         */
        std::unordered_map<std::string, std::unordered_set<std::string>> adj_list = {
            {"a", {"b", "d"}},                                       // abc is 1 SCC, index 0
            {"b", {"c", "f"}}, {"c", {"a", "j"}}, {"d", {"e", "g"}}, // d is 1 SCC, index 1
            {"e", {"f"}},                                            // ef is 1 SCC, index 2
            {"f", {"e"}},      {"g", {"h", "i"}},                    // ghi is 1 SCC, index 3
            {"h", {"g"}},      {"i", {"g"}},      {"j", {"k"}},      // jk is 1 SCC, index 4
            {"k", {"j"}}};

        auto next_star_result = get_next_star_pairs(adj_list);

        // Generate ground truth correct next* relations
        std::unordered_set<std::tuple<std::string, std::string>> correct_next_star_result = {
            // abc SCC, index 0
            {"a", "a"},
            {"a", "b"},
            {"a", "c"},
            {"b", "a"},
            {"b", "b"},
            {"b", "c"},
            {"c", "a"},
            {"c", "b"},
            {"c", "c"},
            // d SCC, index 1
            // AN SCC OF SIZE 1 CANNOT SELF REFER
            // ef SCC, index 2
            {"e", "e"},
            {"e", "f"},
            {"f", "e"},
            {"f", "f"},
            // ghi SCC, index 3
            {"g", "g"},
            {"g", "h"},
            {"g", "i"},
            {"h", "g"},
            {"h", "h"},
            {"h", "i"},
            {"i", "g"},
            {"i", "h"},
            {"i", "i"},
            // jk SCC, index 4
            {"j", "j"},
            {"j", "k"},
            {"k", "j"},
            {"k", "k"},
            // abc SCC to d SCC
            {"a", "d"},
            {"b", "d"},
            {"c", "d"},
            // d SCC to ef SCC
            {"d", "e"},
            {"d", "f"},
            // abc SCC to ef SCC
            {"a", "e"},
            {"a", "f"},
            {"b", "e"},
            {"b", "f"},
            {"c", "e"},
            {"c", "f"},
            // d SCC to ghi SCC
            {"d", "g"},
            {"d", "h"},
            {"d", "i"},
            // abc SCC to ghi SCC
            {"a", "g"},
            {"a", "h"},
            {"a", "i"},
            {"b", "g"},
            {"b", "h"},
            {"b", "i"},
            {"c", "g"},
            {"c", "h"},
            {"c", "i"},
            // abc SCC to jk SCC
            {"a", "j"},
            {"a", "k"},
            {"b", "j"},
            {"b", "k"},
            {"c", "j"},
            {"c", "k"},
        };

        REQUIRE(next_star_result == correct_next_star_result);
    }
}