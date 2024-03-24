#include "qps/evaluators/with_evaluator.hpp"

namespace qps {
auto WithEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_with(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto WithEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), ref_1, ref_2);
}

auto get_attributes(const std::shared_ptr<pkb::ReadFacade>& read_facade, const std::shared_ptr<Synonym>& synonym) {

    bool is_stmt = false;
    if (const auto stmt = std::dynamic_pointer_cast<StmtSynonym>(synonym)) {
        is_stmt = true;
    }

    return overloaded{[read_facade, synonym, is_stmt](const ProcName&) -> std::unordered_set<std::string> {
                          return is_stmt ? read_facade->get_all_calls_callees() : synonym->scan(read_facade);
                      },
                      [read_facade, synonym, is_stmt](const VarName&) -> std::unordered_set<std::string> {
                          if (!is_stmt) {
                              return read_facade->get_variables();
                          }

                          std::unordered_set<std::string> results_set;
                          if (auto read_syn = std::dynamic_pointer_cast<ReadSynonym>(synonym)) {
                              const auto all_stmts = read_syn->scan(read_facade);
                              for (const auto& stmt : all_stmts) {
                                  const auto vars = read_facade->get_vars_modified_by_statement(stmt);
                                  results_set.insert(vars.begin(), vars.end());
                              }
                          } else if (auto print_syn = std::dynamic_pointer_cast<PrintSynonym>(synonym)) {
                              const auto all_stmts = print_syn->scan(read_facade);
                              for (const auto& stmt : all_stmts) {
                                  const auto vars = read_facade->get_vars_used_by_statement(stmt);
                                  results_set.insert(vars.begin(), vars.end());
                              }
                          }

                          return results_set;
                      },
                      [read_facade](const Value&) -> std::unordered_set<std::string> {
                          return read_facade->get_constants();
                      },
                      [read_facade, synonym](const StmtNum&) -> std::unordered_set<std::string> {
                          return synonym->scan(read_facade);
                      }};
}

auto WithEvaluator::eval_with(const AttrRef& attr_1, const AttrRef& attr_2) const -> OutputTable {
    if (!attr_1.type_equals(attr_2)) {
        return Table{};
    }

    auto table = Table{{attr_1.synonym, attr_2.synonym}};
    const auto attribute_extractor = make_attribute_extractor();

    bool is_syn_1_stmt = false;
    if (const auto stmt = std::dynamic_pointer_cast<StmtSynonym>(attr_1.synonym)) {
        is_syn_1_stmt = true;
    }
    bool is_syn_2_stmt = false;
    if (const auto stmt = std::dynamic_pointer_cast<StmtSynonym>(attr_2.synonym)) {
        is_syn_2_stmt = true;
    }

    const auto attributes_set_1 = std::visit(get_attributes(read_facade, attr_1.synonym), attr_1.attr_name);
    const auto attributes_set_2 = std::visit(get_attributes(read_facade, attr_2.synonym), attr_2.attr_name);

    for (const auto& val : attributes_set_1) {
        if (attributes_set_2.find(val) != attributes_set_2.end()) {
            auto result_1 = table.add_row({val, val});
        }
    }

    return table;
}

auto WithEvaluator::eval_with(const AttrRef& attr, const QuotedIdent& quoted_ident) const -> OutputTable {
    auto table = Table{{attr.synonym}};
    const auto attributes = std::visit(get_attributes(read_facade, attr.synonym), attr.attr_name);
    std::unordered_set<std::string> results_set;
    if (const auto call_stmt = std::dynamic_pointer_cast<CallSynonym>(attr.synonym)) {
        for (const auto& attribute : attributes) {
            if (attribute != quoted_ident.get_value()) {
                continue;
            }

            results_set.insert();
        }
    } else if (const auto read_stmt = std::dynamic_pointer_cast<ReadSynonym>(attr.synonym)) {

    } else if (const auto print_stmt = std::dynamic_pointer_cast<PrintSynonym>(attr.synonym)) {
    }

    return table;
}

auto WithEvaluator::eval_with(const AttrRef& attr, const Integer& integer) const -> OutputTable {
    auto table = Table{{attr.synonym}};
    const auto attributes_set = std::visit(get_attributes(read_facade, attr.synonym), attr.attr_name);
    for (const auto& val : attributes_set) {
        if (val == integer.value) {
            table.add_row({val});
        }
    }
    return table;
}

auto WithEvaluator::eval_with(const QuotedIdent& quoted_ident, const AttrRef& attr) const -> OutputTable {
    return eval_with(attr, quoted_ident);
}

auto WithEvaluator::eval_with(const QuotedIdent& quoted_ident_1, const QuotedIdent& quoted_ident_2) const
    -> OutputTable {
    if (quoted_ident_1 == quoted_ident_2) {
        return UnitTable{};
    }

    return Table{};
}

auto WithEvaluator::eval_with(const QuotedIdent& quoted_ident, const Integer& integer) const -> OutputTable {
    return Table{};
}

auto WithEvaluator::eval_with(const Integer& integer, const AttrRef& attr) const -> OutputTable {
    return eval_with(attr, integer);
}

auto WithEvaluator::eval_with(const Integer& integer, const QuotedIdent& quoted_ident) const -> OutputTable {
    return eval_with(quoted_ident, integer);
}

auto WithEvaluator::eval_with(const Integer& integer_1, const Integer& integer_2) const -> OutputTable {
    if (integer_1 == integer_2) {
        return UnitTable{};
    }

    return Table{};
}

} // namespace qps