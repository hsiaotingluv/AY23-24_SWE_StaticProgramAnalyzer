#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"

namespace qps {
class WithEvaluator : public ClauseEvaluator {
    std::shared_ptr<pkb::ReadFacade> read_facade;
    TypedRef ref_1;
    TypedRef ref_2;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. with x.attr = y.attr
    [[nodiscard]] auto eval_with(const qps::AttrRef& attr_1, const qps::AttrRef& attr_2) const -> OutputTable;

    // e.g. with x.attr = "y"
    [[nodiscard]] auto eval_with(const qps::AttrRef& attr, const qps::QuotedIdent& quoted_ident) const -> OutputTable;

    // e.g. with x.attr = 1
    [[nodiscard]] auto eval_with(const qps::AttrRef& attr, const qps::Integer& integer) const -> OutputTable;

    // e.g. with "x" = y.attr
    [[nodiscard]] auto eval_with(const qps::QuotedIdent& quoted_ident, const qps::AttrRef& attr) const -> OutputTable;

    // e.g. with "x" = "y"
    [[nodiscard]] auto eval_with(const qps::QuotedIdent& quoted_ident_1, const qps::QuotedIdent& quoted_ident_2) const
        -> OutputTable;

    // e.g. with "x" = 1
    [[nodiscard]] auto eval_with(const qps::QuotedIdent& quoted_ident, const qps::Integer& integer) const
        -> OutputTable;

    // e.g. with 1 = y.attr
    [[nodiscard]] auto eval_with(const qps::Integer& integer, const qps::AttrRef& attr) const -> OutputTable;

    // e.g. with 1 = "y"
    [[nodiscard]] auto eval_with(const qps::Integer& integer, const qps::QuotedIdent& quoted_ident) const
        -> OutputTable;

    // e.g. with 1 = 1
    [[nodiscard]] auto eval_with(const qps::Integer& integer_1, const qps::Integer& integer_2) const -> OutputTable;

    [[nodiscard]] auto make_attribute_extractor() const {
        return overloaded{[](const std::shared_ptr<Synonym>&) -> std::function<std::string(const std::string&)> {
                              return [](const std::string& x) -> std::string {
                                  return x;
                              };
                          },
                          [&](const AttrRef& attr_ref) -> std::function<std::string(const std::string&)> {
                              const auto synonym = attr_ref.synonym;
                              if (std::dynamic_pointer_cast<PrintSynonym>(synonym) &&
                                  std::holds_alternative<VarName>(attr_ref.attr_name)) {
                                  return [&](const std::string& x) -> std::string {
                                      return *read_facade->get_vars_used_by_statement(x).begin();
                                  };
                              } else if (std::dynamic_pointer_cast<ReadSynonym>(synonym) &&
                                         std::holds_alternative<VarName>(attr_ref.attr_name)) {
                                  return [&](const std::string& x) -> std::string {
                                      return *read_facade->get_vars_modified_by_statement(x).begin();
                                  };
                              } else if (std::dynamic_pointer_cast<CallSynonym>(synonym) &&
                                         std::holds_alternative<ProcName>(attr_ref.attr_name)) {
                                  return [&](const std::string& x) -> std::string {
                                      return read_facade->get_procedure_name_called_by(x);
                                  };
                              } else {
                                  return [](const std::string& x) -> std::string {
                                      return x;
                                  };
                              }
                          }};
    }

  public:
    WithEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, TypedRef ref_1, TypedRef ref_2)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), ref_1(std::move(ref_1)), ref_2(std::move(ref_2)) {
    }

    [[nodiscard]] auto evaluate() const -> OutputTable override;
};

} // namespace qps