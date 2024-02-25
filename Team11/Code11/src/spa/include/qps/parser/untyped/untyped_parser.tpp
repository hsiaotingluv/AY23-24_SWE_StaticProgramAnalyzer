#pragma once

#include "common/tokeniser/runner.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/query.hpp"
#include "qps/template_utils.hpp"
#include "qps/tokeniser/tokeniser.hpp"

#include "qps/parser/errors.hpp"

// Forward declarations of helper functions
namespace qps::untyped::detail {
template <typename Head, typename... Tails>
auto parse_declarations(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                        TypeList<Head, Tails...> supported_synonyms)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::const_iterator>>;

template <typename SupportedResultParsers, typename SupportedClauseParsers>
auto build_untyped_query(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<UntypedQuery>;
} // namespace qps::untyped::detail

namespace qps::untyped {
template <typename SupportedSynonyms, typename SupportedSelectParsers, typename SupportedClauseParsers>
class UntypedParser {
  private:
    static inline const auto tokeniser_runner =
        tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

  public:
    static auto parse(std::string query) -> std::variant<std::tuple<Synonyms, untyped::UntypedQuery>, SyntaxError> {
        const auto maybe_tokens = [&query]() -> std::variant<std::vector<Token>, SyntaxError> {
            try {
                return tokeniser_runner.apply_tokeniser(std::move(query));
            } catch (const std::exception& e) {
                return SyntaxError{e.what()};
            }
        }();

        if (std::holds_alternative<SyntaxError>(maybe_tokens)) {
            return std::get<SyntaxError>(maybe_tokens);
        }
        const auto& tokens = std::get<std::vector<Token>>(maybe_tokens);
        auto begin = tokens.begin();
        const auto end = tokens.end();

        // Parse declarations
        const auto maybe_declared_synonyms = detail::parse_declarations(begin, end, SupportedSynonyms{});
        if (!maybe_declared_synonyms.has_value()) {
            return SyntaxError{"Syntax error: unable to declare synonyms"};
        }

        const auto& [declared_synonyms, rest] = maybe_declared_synonyms.value();
        begin = rest;

        // Parse remaining
        const auto maybe_remaining =
            detail::build_untyped_query<SupportedSelectParsers, SupportedClauseParsers>(begin, end);

        if (!maybe_remaining.has_value()) {
            const auto remaining_str = [begin, end]() -> std::string {
                const auto remaining = std::vector<Token>(begin, end);
                std::string str;
                for (const auto& token : remaining) {
                    str += token.content;
                }
                return str;
            }();
            return SyntaxError{"Syntax error: unable to parse remaining: " + remaining_str};
        }
        return std::make_tuple(declared_synonyms, maybe_remaining.value());
    }
};
} // namespace qps::untyped

namespace qps::untyped::detail {
template <typename SynonymType>
auto build_synonyms(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::const_iterator>> {
    if (std::distance(it, end) < 2) {
        return std::nullopt;
    }

    // Match keyword
    constexpr auto expected = SynonymType::keyword;
    const auto keyword = *it;
    if (!is_keyword(keyword, expected)) {
        return std::nullopt;
    }
    it = std::next(it);

    // Try to consume first synonym
    Synonyms synonyms{};

    auto maybe_synonym = *it;
    if (!is_string(maybe_synonym)) {
        return std::nullopt;
    }
    it = std::next(it);
    synonyms.push_back(std::make_shared<SynonymType>(IDENT{maybe_synonym.content}));

    // Try to consume remaining synonyms with pattern: ',' <synonym>
    while (it != end) {
        if (!is_comma(*it)) {
            break;
        }
        it = std::next(it);

        if (it == end) {
            return std::nullopt;
        }

        const auto synonym = *it;
        if (!is_string(synonym)) {
            break;
        }
        it = std::next(it);
        synonyms.emplace_back(std::make_shared<SynonymType>(IDENT{synonym.content}));
    }

    // Consume ';' delimiter if it exists
    if (it != end && is_semicolon(*it)) {
        it = std::next(it);
    }
    return std::make_tuple(synonyms, it);
}

template <typename T>
auto try_declare_synonyms(Synonyms& synonyms, std::vector<Token>::const_iterator it,
                          const std::vector<Token>::const_iterator& end)
    -> std::optional<std::vector<Token>::const_iterator> {
    const auto maybe_decl = build_synonyms<T>(it, end);
    if (maybe_decl.has_value()) {
        const auto [decl, rest] = maybe_decl.value();
        synonyms.insert(synonyms.end(), std::make_move_iterator(decl.begin()), std::make_move_iterator(decl.end()));
        return rest;
    }
    return std::nullopt;
}

inline auto parse_declarations_rec(Synonyms&, std::vector<Token>::const_iterator,
                                   const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::vector<Token>::const_iterator> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto parse_declarations_rec(Synonyms& synonyms, std::vector<Token>::const_iterator it,
                            const std::vector<Token>::const_iterator& end, TypeList<Head, Tails...>)
    -> std::optional<std::vector<Token>::const_iterator> {
    auto maybe_it = try_declare_synonyms<Head>(synonyms, it, end);
    return maybe_it.has_value() ? maybe_it.value() : parse_declarations_rec(synonyms, it, end, TypeList<Tails...>{});
}

template <typename Head, typename... Tails>
auto parse_declarations(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                        TypeList<Head, Tails...> supported_synonyms)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::const_iterator>> {
    auto declared_synonyms = Synonyms{};
    while (true) {
        const auto maybe_it = parse_declarations_rec(declared_synonyms, it, end, supported_synonyms);
        if (!maybe_it.has_value()) {
            break;
        }
        it = maybe_it.value();
    }
    return !declared_synonyms.empty() ? std::make_optional(std::make_tuple(declared_synonyms, it)) : std::nullopt;
}

inline auto parse_result_cl(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto parse_result_cl(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                     TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>> {
    const auto maybe_select = Head::parse(it, end);
    if (maybe_select.has_value()) {
        const auto& [synonym, rest] = maybe_select.value();
        return std::make_tuple(synonym, rest);
    }
    return parse_result_cl(it, end, TypeList<Tails...>{});
}

auto parse_clause(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<UntypedClause, std::vector<Token>::const_iterator>>;

template <typename Head, typename... Tails>
auto parse_clause(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                  TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedClause, std::vector<Token>::const_iterator>> {
    const auto maybe_clause = Head::parse(it, end);
    if (maybe_clause.has_value()) {
        return maybe_clause;
    }

    return parse_clause(it, end, TypeList<Tails...>{});
}

template <typename SupportedResultParsers, typename SupportedClauseParsers>
auto build_untyped_query(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<UntypedQuery> {
    const auto maybe_reference = parse_result_cl(it, end, SupportedResultParsers{});
    if (!maybe_reference.has_value()) {
        return std::nullopt;
    }

    const auto& [reference, rest] = maybe_reference.value();
    it = rest;

    auto clauses = std::vector<UntypedClause>{};
    while (it != end) {
        const auto maybe_clause = parse_clause(it, end, SupportedClauseParsers{});
        if (!maybe_clause.has_value()) {
            break;
        }

        const auto& [clause, rest] = maybe_clause.value();
        clauses.push_back(clause);
        it = rest;
    }

    if (it == end) {
        return std::make_tuple(reference, clauses);
    }
    return std::nullopt;
}
} // namespace qps::untyped::detail