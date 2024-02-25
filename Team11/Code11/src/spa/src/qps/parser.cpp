#include "qps/parser.hpp"
#include "qps/parser/errors.hpp"
#include "qps/parser/semantic_validator.hpp"

#include <optional>
#include <variant>

namespace qps {

auto to_query(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query)
    -> std::optional<qps::Query> {
    return std::holds_alternative<qps::Query>(maybe_query) ? std::make_optional(std::get<qps::Query>(maybe_query))
                                                           : std::nullopt;
}

auto is_syntax_error(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query) -> bool {
    return std::holds_alternative<qps::SyntaxError>(maybe_query);
}

auto is_semantic_error(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query) -> bool {
    return std::holds_alternative<qps::SemanticError>(maybe_query);
}
} // namespace qps
