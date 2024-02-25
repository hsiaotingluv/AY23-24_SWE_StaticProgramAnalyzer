#include "qps/parser.hpp"
#include "qps/parser/errors.hpp"
#include "qps/parser/semantic_validator.hpp"
#include "qps/parser/untyped/untyped_parser.hpp"

#include <optional>
#include <variant>

namespace qps {
auto Parser::parse(std::string query) -> std::variant<Query, SyntaxError, SemanticError> {
    const auto maybe_parsed = qps::untyped::UntypedParser::parse(std::move(query));

    if (std::holds_alternative<SyntaxError>(maybe_parsed)) {
        return std::get<SyntaxError>(maybe_parsed);
    }

    const auto& [declared_synonyms, untyped_query] =
        std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(maybe_parsed);
    const auto maybe_query = qps::DefaultSemanticAnalyser::validate(declared_synonyms, untyped_query);

    if (std::holds_alternative<SemanticError>(maybe_query)) {
        return std::get<SemanticError>(maybe_query);
    }

    return std::get<Query>(maybe_query);
}

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
