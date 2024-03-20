#include "qps/evaluators/relationship/next_t_evaluator.hpp"
#include <stack>

namespace qps {

auto NextTEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_next_t(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto NextTEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), next_t.stmt1, next_t.stmt2);
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                 const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    // TODO:
}

std::unordered_set<std::string>
get_all_transitive_from_node(const std::string& node,
                             const std::unordered_map<std::string, std::unordered_set<std::string>>& map) {
    std::unordered_set<std::string> result;
    std::stack<std::string> stack;

    stack.push(node);

    while (!stack.empty()) {
        // Get top of stack
        const auto current = stack.top();
        stack.pop();

        // If already visited, skip
        if (result.find(current) != result.end()) {
            continue;
        }

        // Else, add to result
        result.insert(current);

        // Add all next nodes to stack
        const auto next_nodes = map.at(current);
        for (const auto& next_node : next_nodes) {
            stack.push(next_node);
        }
    }

    return result;
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const Integer& stmt_num_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);
    auto table = Table{{stmt_syn_1}};
    const auto next_reverse_map = read_facade->get_all_next_reverse();

    auto new_rows = get_all_transitive_from_node(stmt_num_2.value, next_reverse_map);

    for (const auto& row : new_rows) {
        if (relevant_stmts.find(row) != relevant_stmts.end()) {
            table.add_row({row});
        }
    }

    return table;
}

auto NextTEvaluator::eval_next_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const WildCard&) const -> OutputTable {
    const auto relevant_stmts = stmt_syn_1->scan(read_facade);

    auto table = Table{{stmt_syn_1}};
    auto all_stmts = read_facade->get_all_next_keys();

    for (const auto& stmt : all_stmts) {
        if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
            table.add_row({stmt});
        }
    }

    return table;
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
    -> OutputTable {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);
    auto table = Table{{stmt_syn_2}};

    const auto next_map = read_facade->get_all_next();

    auto new_rows = get_all_transitive_from_node(stmt_num_1.value, next_map);

    for (const auto& row : new_rows) {
        if (relevant_stmts.find(row) != relevant_stmts.end()) {
            table.add_row({row});
        }
    }

    return table;
}

bool has_transitive_rs(const std::string& node1, const std::string& node2,
                       const std::unordered_map<std::string, std::unordered_set<std::string>>& map) {
    std::unordered_set<std::string> visited;
    std::stack<std::string> stack;

    for (const auto& child_node : map.at(node1)) {
        stack.push(child_node);
    }

    while (!stack.empty()) {
        // Get top of stack
        const auto current = stack.top();
        stack.pop();

        if (current == node2) {
            return true;
        }

        // If already visited, skip
        if (visited.find(current) != visited.end()) {
            continue;
        }

        // Else, add to visited
        visited.insert(current);

        // Add all next nodes to stack
        const auto next_nodes = map.at(current);
        for (const auto& next_node : next_nodes) {
            stack.push(next_node);
        }
    }

    return false;
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const Integer& stmt_num_2) const -> OutputTable {
    const auto next_map = read_facade->get_all_next();

    bool has_transitive = has_transitive_rs(stmt_num_1.value, stmt_num_2.value, next_map);

    if (has_transitive) {
        return UnitTable{};
    }

    return Table{};
}

auto NextTEvaluator::eval_next_t(const Integer& stmt_num_1, const WildCard&) const -> OutputTable {
    bool has_next_rs = read_facade->contains_next_key(stmt_num_1.value);

    if (has_next_rs) {
        return UnitTable{};
    }

    return Table{};
}

auto NextTEvaluator::eval_next_t(const WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable {
    const auto relevant_stmts = stmt_syn_2->scan(read_facade);

    auto table = Table{{stmt_syn_2}};
    auto all_stmts = read_facade->get_all_next_values();

    for (const auto& stmt : all_stmts) {
        if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
            table.add_row({stmt});
        }
    }

    return table;
}

auto NextTEvaluator::eval_next_t(const WildCard&, const Integer& stmt_num_2) const -> OutputTable {
    bool has_next_rs = read_facade->contains_next_value(stmt_num_2.value);

    if (has_next_rs) {
        return UnitTable{};
    }

    return Table{};
}

auto NextTEvaluator::eval_next_t(const WildCard&, const WildCard&) const -> OutputTable {
    if (read_facade->has_next_relation()) {
        return UnitTable{};
    }

    return Table{};
}
} // namespace qps