#pragma once

#include "qps/parser/errors.hpp"
#include "qps/parser/semantic_validator.hpp"

namespace qps {
class Parser {
  public:
    static auto parse(std::string query) -> std::variant<Query, SyntaxError, SemanticError>;
};

auto to_query(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query)
    -> std::optional<qps::Query>;

auto is_syntax_error(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query) -> bool;

auto is_semantic_error(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query) -> bool;

} // namespace qps
