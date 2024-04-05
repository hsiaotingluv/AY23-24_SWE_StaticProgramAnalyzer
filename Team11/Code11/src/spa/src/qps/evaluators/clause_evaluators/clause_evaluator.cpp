#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"

auto qps::ClauseEvaluator::negate_result(qps::OutputTable output_table) const -> qps::OutputTable {
    return std::visit(overloaded{[](const UnitTable&) -> qps::OutputTable {
                                     return Table{};
                                 },
                                 [this](const Table& table) -> qps::OutputTable {
                                     if (is_empty(table)) {
                                         return UnitTable{};
                                     }
                                     auto column_synonyms = table.get_column();
                                     // [EXPENSIVE] for each synonym, do a synonym.scan(read_facade) and put into a
                                     // table. cross join all these tables.
                                     auto curr_table = OutputTable{UnitTable{}};
                                     for (const auto& synonym : column_synonyms) {
                                         const auto all_stored_synonyms = synonym->scan(read_facade);
                                         auto new_table = Table{{synonym}};
                                         for (const auto& stored_syn : all_stored_synonyms) {
                                             new_table.add_row({stored_syn});
                                         }
                                         curr_table = join(std::move(curr_table), std::move(new_table));
                                     }
                                     // TODO: subtract original table from this new table and return that result
                                     return subtract(std::move(curr_table), table, read_facade);
                                 }},
                      output_table);
}

auto qps::ClauseEvaluator::evaluate() const -> OutputTable {
    if (is_negated) {
        return negate_result(evaluate_positive());
    } else {
        return evaluate_positive();
    }
}
