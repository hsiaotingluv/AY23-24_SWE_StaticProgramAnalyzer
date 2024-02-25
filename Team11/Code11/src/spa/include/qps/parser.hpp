#pragma once

#include "qps/parser/errors.hpp"
#include "qps/parser/semantic_analyser.hpp"
#include "qps/parser/untyped/untyped_parser.hpp"

namespace qps {
template <typename UntypedParser, typename SemanticAnalyser>
class Parser {
  public:
    static auto parse(std::string query) -> std::variant<Query, SyntaxError, SemanticError> {
        const auto maybe_parsed = UntypedParser::parse(std::move(query));

        if (std::holds_alternative<SyntaxError>(maybe_parsed)) {
            return std::get<SyntaxError>(maybe_parsed);
        }

        const auto& [declared_synonyms, untyped_query] =
            std::get<std::tuple<Synonyms, untyped::UntypedQuery>>(maybe_parsed);
        const auto maybe_query = SemanticAnalyser::analyse(declared_synonyms, untyped_query);

        if (std::holds_alternative<SemanticError>(maybe_query)) {
            return std::get<SemanticError>(maybe_query);
        }

        return std::get<Query>(maybe_query);
    }
};
} // namespace qps

namespace qps {
using DefaultParser = Parser<untyped::DefaultUntypedParser, DefaultSemanticAnalyser>;

auto to_query(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query)
    -> std::optional<qps::Query>;

auto is_syntax_error(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query) -> bool;

auto is_semantic_error(const std::variant<qps::Query, qps::SyntaxError, qps::SemanticError>& maybe_query) -> bool;

} // namespace qps
