#include "qps/evaluators/clause_evaluators/with_evaluator.hpp"

namespace qps {
auto WithEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_with(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto WithEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), ref_1, ref_2);
}

auto WithEvaluator::eval_with(const AttrRef& attr_1, const AttrRef& attr_2) const -> OutputTable {
    if (!attr_1.type_equals(attr_2)) {
        // Different types of attributes will never be equal!
        return Table{};
    }

    auto table = Table{{attr_1.synonym, attr_2.synonym}};
    const auto attribute_extractor = AttributeExtractor(read_facade)();

    const auto extractor_1 = attribute_extractor(attr_1);
    const auto extractor_2 = attribute_extractor(attr_2);

    const auto values_set_1 = attr_1.synonym->scan(read_facade);
    const auto values_set_2 = attr_2.synonym->scan(read_facade);

    for (const auto& val_1 : values_set_1) {
        for (const auto& val_2 : values_set_2) {
            if (extractor_1(val_1) == extractor_2(val_2)) {
                table.add_row({val_1, val_2});
            }
        }
    }

    return table;
}

auto WithEvaluator::eval_with(const AttrRef& attr, const QuotedIdent& quoted_ident) const -> OutputTable {
    auto table = Table{{attr.synonym}};

    const auto attribute_extractor = AttributeExtractor(read_facade)();
    const auto extractor = attribute_extractor(attr);
    const auto values_set = attr.synonym->scan(read_facade);

    for (const auto& val : values_set) {
        if (extractor(val) == quoted_ident.get_value()) {
            table.add_row({val});
        }
    }
    return table;
}

auto WithEvaluator::eval_with(const AttrRef& attr, const Integer& integer) const -> OutputTable {
    // Functionally the same as for quoted_ident
    return eval_with(attr, QuotedIdent{(integer.value)});
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

auto WithEvaluator::eval_with(const QuotedIdent&, const Integer&) const -> OutputTable {
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