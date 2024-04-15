#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/hashable_tuple.h"

// Given one unordered set of strings, create all permutations of the set
std::vector<std::pair<std::string, std::string>> create_permutations(const std::unordered_set<std::string>& set);

// Given two unordered sets of strings, create all permutations of the two sets
std::vector<std::pair<std::string, std::string>> create_permutations(const std::unordered_set<std::string>& set1,
                                                                     const std::unordered_set<std::string>& set2);

/***
 * Given an adjacency list of type std::unordered_map<std::string, std::unordered_set<std::string>>,
 * use Tarjan's algorithm to find all strongly connected components in the graph.
 */
std::vector<std::unordered_set<std::string>>
tarjan_scc(const std::unordered_map<std::string, std::unordered_set<std::string>>& adj_list);

// Generate the mapping between index and node name, and vice versa
std::tuple<std::unordered_map<int, std::unordered_set<std::string>>, std::unordered_map<std::string, int>>
generate_maps(const std::vector<std::unordered_set<std::string>>& scc);

// Generate adjacency list according to SCC index
std::unordered_map<int, std::unordered_set<int>>
generate_adj_list(const std::unordered_map<std::string, std::unordered_set<std::string>>& adj_list,
                  const std::unordered_map<std::string, int>& reverse_index_map);

// Given an adjacency list, return the toposorted order of the graph
std::vector<int> toposort(const std::unordered_map<int, std::unordered_set<int>>& adj_list);

// Given an adjacency list, return the correct next* relationship pairs
std::unordered_set<std::tuple<std::string, std::string>>
get_next_star_pairs(const std::unordered_map<std::string, std::unordered_set<std::string>>& adj_list);

// Check whether there's a transitive rs
bool has_transitive_rs(
    const std::string& node1, const std::unordered_set<std::string>& end_nodes,
    const std::unordered_map<std::string, std::unordered_set<std::string>>& map,
    const std::function<bool(const std::string&)>& start_node_cond =
        [](const std::string&) {
            return true;
        },
    const std::function<bool(const std::string&)>& end_node_cond =
        [](const std::string&) {
            return true;
        },
    const std::function<bool(const std::string&)>& intermediate_node_cond =
        [](const std::string&) {
            return true;
        });

std::unordered_set<std::string> get_all_transitive_from_node(
    const std::string& node, const std::unordered_map<std::string, std::unordered_set<std::string>>& map,
    const std::function<bool(const std::string&)>& start_node_cond =
        [](const std::string&) {
            return true;
        },
    const std::function<bool(const std::string&)>& end_node_cond =
        [](const std::string&) {
            return true;
        },
    const std::function<bool(const std::string&)>& intermediate_node_cond =
        [](const std::string&) {
            return true;
        });