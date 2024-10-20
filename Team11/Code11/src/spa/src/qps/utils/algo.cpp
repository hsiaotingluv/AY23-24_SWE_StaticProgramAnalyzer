#include <iostream>
#include <stack>
#include <stdexcept>

#include "common/utils/algo.h"
#include "qps/utils/algo.h"

// Given one unordered set of strings, create all permutations of the set
std::vector<std::pair<std::string, std::string>> create_permutations(const std::unordered_set<std::string>& set) {
    if (set.size() == 1) {
        return {};
    }

    std::vector<std::pair<std::string, std::string>> result;

    for (const auto& elem1 : set) {
        for (const auto& elem2 : set) {
            result.emplace_back(elem1, elem2);
        }
    }

    return result;
}

// Given two unordered sets of strings, create all permutations of the two sets
std::vector<std::pair<std::string, std::string>> create_permutations(const std::unordered_set<std::string>& set1,
                                                                     const std::unordered_set<std::string>& set2) {
    std::vector<std::pair<std::string, std::string>> result;

    for (const auto& elem1 : set1) {
        for (const auto& elem2 : set2) {
            result.emplace_back(elem1, elem2);
        }
    }

    return result;
}

/***
 * Given an adjacency list of type std::unordered_map<std::string, std::unordered_set<std::string>>,
 * use Tarjan's algorithm to find all strongly connected components in the graph.
 */
std::vector<std::unordered_set<std::string>>
tarjan_scc(const std::unordered_map<std::string, std::unordered_set<std::string>>& adj_list) {
    std::vector<std::unordered_set<std::string>> result;
    std::unordered_map<std::string, int> index;
    std::unordered_map<std::string, int> lowlink;
    std::unordered_set<std::string> on_stack;
    std::stack<std::string> stack;
    int index_counter = 0;

    // Helper function to perform the depth first search
    std::function<void(const std::string&)> dfs = [&](const std::string& node) {
        index[node] = index_counter;
        lowlink[node] = index_counter;
        index_counter++;
        stack.push(node);
        on_stack.insert(node);

        auto it = adj_list.find(node);
        if (it != adj_list.end()) {
            for (const auto& child : it->second) {
                if (index.find(child) == index.end()) {
                    dfs(child);
                    lowlink[node] = std::min(lowlink[node], lowlink[child]);
                } else if (on_stack.find(child) != on_stack.end()) {
                    lowlink[node] = std::min(lowlink[node], index[child]);
                }
            }
        }

        if (lowlink[node] == index[node]) {
            std::unordered_set<std::string> scc;
            std::string current;
            do {
                current = stack.top();
                stack.pop();
                on_stack.erase(current);
                scc.insert(current);
            } while (current != node);
            result.push_back(scc);
        }
    };

    for (const auto& [node, _] : adj_list) {
        if (index.find(node) == index.end()) {
            dfs(node);
        }
    }

    return result;
}

// Generate the mapping between index and node name, and vice versa
std::tuple<std::unordered_map<int, std::unordered_set<std::string>>, std::unordered_map<std::string, int>>
generate_maps(const std::vector<std::unordered_set<std::string>>& scc) {
    std::unordered_map<int, std::unordered_set<std::string>> result;
    std::unordered_map<std::string, int> reverse_result;

    for (std::size_t i = 0; i < scc.size(); i++) {
        result[static_cast<int>(i)] = scc[i];
        for (const auto& elem : scc[i]) {
            reverse_result[elem] = static_cast<int>(i);
        }
    }

    return std::tuple{result, reverse_result};
}

// Generate adjacency list according to SCC index
std::unordered_map<int, std::unordered_set<int>>
generate_adj_list(const std::unordered_map<std::string, std::unordered_set<std::string>>& adj_list,
                  const std::unordered_map<std::string, int>& reverse_index_map) {
    std::unordered_map<int, std::unordered_set<int>> result;

    for (const auto& [node, children] : adj_list) {
        // str (node name) -> int (index)
        auto it = reverse_index_map.find(node);
        if (it != reverse_index_map.end()) {
            // For every child
            for (const auto& child : children) {
                // Get index of child
                auto it_child = reverse_index_map.find(child);
                // If not the same as index of parent
                if (it_child != reverse_index_map.end() && it_child->second != it->second) {
                    // Add to adjacency list
                    result[it->second].insert(it_child->second);
                }
            }
        }
    }

    return result;
}

// Given an adjacency list, return the toposorted order of the graph
std::vector<int> toposort(const std::unordered_map<int, std::unordered_set<int>>& adj_list) {
    std::vector<int> result;
    std::unordered_set<int> visited;
    std::unordered_set<int> visiting;

    std::function<void(int)> dfs = [&](int node) {
        visiting.insert(node);
        auto it = adj_list.find(node);
        if (it != adj_list.end()) {
            for (const auto& child : it->second) {
                if (visiting.find(child) != visiting.end()) {
                    throw std::runtime_error("Cycle detected");
                } else if (visited.find(child) == visited.end()) {
                    dfs(child);
                }
            }
        }
        visiting.erase(node);
        visited.insert(node);
        result.push_back(node);
    };

    for (const auto& [node, _] : adj_list) {
        if (visited.find(node) == visited.end()) {
            dfs(node);
        }
    }

    return result;
}

// Given an adjacency list, return the correct next* relationship pairs
std::unordered_set<std::tuple<std::string, std::string>>
get_next_star_pairs(const std::unordered_map<std::string, std::unordered_set<std::string>>& adj_list) {
    auto scc = tarjan_scc(adj_list);
    auto [index_map, reverse_index_map] = generate_maps(scc);
    auto adj_list_index = generate_adj_list(adj_list, reverse_index_map);

    std::vector<std::tuple<int, int>> relationships;

    for (const auto& pair : adj_list_index) {
        for (const auto& child : pair.second) {
            relationships.emplace_back(pair.first, child);
        }
    }

    auto topo = toposort(adj_list_index);

    std::sort(relationships.begin(), relationships.end(), OrderingByIndexMap{topo});

    // Create the transitive map at index level
    std::unordered_map<int, std::unordered_set<int>> transitive_map;

    // populate transitive_map, starting from the last element of relationships
    for (auto it = relationships.rbegin(); it != relationships.rend(); ++it) {
        const auto& [s1, s2] = *it;
        transitive_map[s1].insert(s2);

        // add all the star relationships from s2 to s1
        for (const auto& s3 : transitive_map[s2]) {
            transitive_map[s1].insert(s3);
        }
    }

    // next_star_result is a set of pairs
    auto next_star_result = std::unordered_set<std::tuple<std::string, std::string>>{};

    // For all nodes within the same SCC, all possible permutations goes to next_star_result
    for (const auto& scc_set : scc) {
        auto permutations = create_permutations(scc_set);
        for (const auto& [a, b] : permutations) {
            next_star_result.insert(std::make_tuple(a, b));
        }
    }

    // For all SCCs connected as indicated by transitive_map, permutations
    // between the two SCCs goes to next_star_result
    for (const auto& [s1, s2_set] : transitive_map) {
        for (const auto& s2 : s2_set) {
            auto permutations = create_permutations(scc[s1], scc[s2]);
            for (const auto& [a, b] : permutations) {
                next_star_result.insert(std::make_tuple(a, b));
            }
        }
    }

    return next_star_result;
}

// Check whether there's a transitive rs
bool has_transitive_rs(const std::string& node1, const std::unordered_set<std::string>& end_nodes,
                       const std::unordered_map<std::string, std::unordered_set<std::string>>& map,
                       const std::function<bool(const std::string&)>& start_node_cond,
                       const std::function<bool(const std::string&)>& end_node_cond,
                       const std::function<bool(const std::string&)>& intermediate_node_cond) {
    if (!start_node_cond(node1)) {
        return false;
    }

    // remove end_node that do not adhere to end_node_cond
    std::unordered_set<std::string> valid_end_nodes;
    for (const auto& end_node : end_nodes) {
        if (end_node_cond(end_node)) {
            valid_end_nodes.insert(end_node);
        }
    }

    if (valid_end_nodes.empty()) {
        return false;
    }

    std::unordered_set<std::string> visited;
    std::stack<std::string> stack;

    auto it = map.find(node1);
    if (it != map.end()) {
        for (const auto& child_node : it->second) {
            stack.push(child_node);
        }
    }

    while (!stack.empty()) {
        // Get top of stack
        const auto current = stack.top();
        stack.pop();

        if (valid_end_nodes.find(current) != valid_end_nodes.end()) {
            return true;
        }

        // If already visited or does not pass the intermediate node condition, skip
        if (visited.find(current) != visited.end() || !intermediate_node_cond(current)) {
            continue;
        }

        // Else, add to visited
        visited.insert(current);

        // Add all next nodes to stack
        auto it_current = map.find(current);
        if (it_current != map.end()) {
            for (const auto& next_node : it_current->second) {
                stack.push(next_node);
            }
        }
    }

    return false;
}

std::unordered_set<std::string>
get_all_transitive_from_node(const std::string& node,
                             const std::unordered_map<std::string, std::unordered_set<std::string>>& map,
                             const std::function<bool(const std::string&)>& start_node_cond,
                             const std::function<bool(const std::string&)>& end_node_cond,
                             const std::function<bool(const std::string&)>& intermediate_node_cond) {
    if (!start_node_cond(node)) {
        return {};
    }

    std::unordered_set<std::string> result;
    std::unordered_set<std::string> visited;
    std::stack<std::string> stack;

    auto it = map.find(node);
    if (it != map.end()) {
        for (const auto& next_node : it->second) {
            stack.push(next_node);
        }
    }

    while (!stack.empty()) {
        // Get top of stack
        const auto current = stack.top();
        stack.pop();

        // If passes end_node_cond, add to result
        if (end_node_cond(current)) {
            result.insert(current);
        }

        // If already visited or does not pass intermediate_node_cond, skip
        if (visited.find(current) != visited.end() || !intermediate_node_cond(current)) {
            continue;
        }

        // Else, add to visited
        visited.insert(current);

        // Add all next nodes to stack
        auto it_current = map.find(current);
        if (it_current != map.end()) {
            for (const auto& next_node : it_current->second) {
                stack.push(next_node);
            }
        }
    }

    return result;
}