#pragma once

#include "common/tokeniser/runner.hpp"
#include "qps/parser/concepts.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/parser_helper.hpp"
#include "qps/template_utils.hpp"
#include "qps/tokeniser/tokeniser.hpp"

#include "qps/parser/errors.hpp"
#include <iterator>
#include <optional>
#include <sstream>
#include <tuple>
#include <type_traits>

// Forward declarations of helper functions
namespace qps::untyped::detail {
template <typename Head, typename... Tails, std::enable_if_t<is_parseable_v<Head>, bool> = true>
auto parse_declarations(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                        TypeList<Head, Tails...> supported_synonyms)
    -> std::tuple<Synonyms, std::vector<Token>::const_iterator>;

template <typename SupportedResultParsers, typename SupportedClauseParsers, typename UntypedReferenceType,
          typename UntypedClauseType, std::enable_if_t<are_parsers_v<SupportedResultParsers>, bool> = true>
auto build_untyped_query(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedReferenceType, std::vector<UntypedClauseType>>>;

} // namespace qps::untyped::detail

namespace qps::untyped {
template <typename SupportedSynonyms, typename SupportedSelectParsers, typename SupportedClauseParsers>
class UntypedParser {
    // Ensure that SupportedSynonyms are all parseable
    static_assert(are_parseables_v<SupportedSynonyms>, "SupportedSynonyms must be empty or contain only parseables");

    // Ensure that SupportedSelectParsers is a non-empty typelist of parsers
    static_assert(are_parsers_v<SupportedSelectParsers>,
                  "SupportedSelectParsers must be empty or contain only parsers");

    // Ensure that SupportedClauseParsers is a non-empty typelist of parsers
    static_assert(are_parsers_v<SupportedClauseParsers>,
                  "SupportedClauseParsers must be empty or contain only parsers");

  private:
    static inline const auto tokeniser_runner =
        tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

  public:
    using UntypedReferenceType = type_list_to_variant_t<get_return_type_t<SupportedSelectParsers>>;
    using UntypedClauseType = type_list_to_variant_t<get_return_type_t<SupportedClauseParsers>>;
    using UntypedQueryType = std::tuple<UntypedReferenceType, std::vector<UntypedClauseType>>;

    static auto parse(std::string query) -> std::variant<std::tuple<Synonyms, UntypedQueryType>, SyntaxError> {
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
        const auto& [declared_synonyms, rest] = detail::parse_declarations(begin, end, SupportedSynonyms{});
        begin = rest;

        // Parse remaining
        const auto maybe_remaining = detail::build_untyped_query<SupportedSelectParsers, SupportedClauseParsers,
                                                                 UntypedReferenceType, UntypedClauseType>(begin, end);

        if (!maybe_remaining.has_value()) {
            const auto remaining_str = [begin, end]() -> std::string {
                const auto remaining = std::vector<Token>(begin, end);
                std::stringstream ss;
                for (const auto& token : remaining) {
                    ss << token.content << " ";
                }
                return ss.str();
            }();
            return SyntaxError{"Syntax error: unable to parse remaining: " + remaining_str};
        }
        return std::make_tuple(declared_synonyms, maybe_remaining.value());
    }
};
} // namespace qps::untyped

namespace qps::untyped::detail {
template <typename SynonymType, std::enable_if_t<is_parseable_v<SynonymType>, int> = 0>
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
            return std::nullopt;
        }
        it = std::next(it);
        synonyms.emplace_back(std::make_shared<SynonymType>(IDENT{synonym.content}));
    }

    // Expect ';' delimiter
    if (it == end || !is_semicolon(*it)) {
        return std::nullopt;
    }
    it = std::next(it);
    return std::make_tuple(synonyms, it);
}

template <typename SynonymType, std::enable_if_t<is_parseable_v<SynonymType>, bool> = true>
auto try_declare_synonyms(Synonyms& synonyms, std::vector<Token>::const_iterator it,
                          const std::vector<Token>::const_iterator& end)
    -> std::optional<std::vector<Token>::const_iterator> {
    const auto maybe_decl = build_synonyms<SynonymType>(it, end);
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

template <typename Head, typename... Tails, std::enable_if_t<is_parseable_v<Head>, bool> = true>
auto parse_declarations_rec(Synonyms& synonyms, std::vector<Token>::const_iterator it,
                            const std::vector<Token>::const_iterator& end, TypeList<Head, Tails...>)
    -> std::optional<std::vector<Token>::const_iterator> {
    auto maybe_it = try_declare_synonyms<Head>(synonyms, it, end);
    return maybe_it.has_value() ? maybe_it.value() : parse_declarations_rec(synonyms, it, end, TypeList<Tails...>{});
}

template <typename Head, typename... Tails, std::enable_if_t<is_parseable_v<Head>, bool>>
auto parse_declarations(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                        TypeList<Head, Tails...> supported_synonyms)
    -> std::tuple<Synonyms, std::vector<Token>::const_iterator> {
    auto declared_synonyms = Synonyms{};
    while (true) {
        const auto maybe_it = parse_declarations_rec(declared_synonyms, it, end, supported_synonyms);
        if (!maybe_it.has_value()) {
            break;
        }
        it = maybe_it.value();
    }
    return std::make_tuple(declared_synonyms, it);
}

template <typename UntypedReference>
auto parse_result_cl(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename UntypedReference, typename Head, typename... Tails, std::enable_if_t<is_parser_v<Head>, bool> = true>
auto parse_result_cl(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                     TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>> {
    const auto maybe_select = Head::parse(it, end);
    if (maybe_select.has_value()) {
        const auto& [synonym, rest] = maybe_select.value();
        return std::make_tuple(synonym, rest);
    }
    return parse_result_cl<UntypedReference>(it, end, TypeList<Tails...>{});
}

template <typename UntypedClauseType>
auto parse_clause(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<std::vector<UntypedClauseType>, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename UntypedClauseType, typename Head, typename... Tails,
          std::enable_if_t<is_parser_v<Head>, bool> = true>
auto parse_clause(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                  TypeList<Head, Tails...>)
    -> std::optional<std::tuple<std::vector<UntypedClauseType>, std::vector<Token>::const_iterator>> {
    const auto maybe_clause = Head::parse(it, end);
    if (maybe_clause.has_value()) {
        auto& [clauses, rest] = maybe_clause.value();
        auto variant_clauses = std::vector<UntypedClauseType>{};
        variant_clauses.reserve(clauses.size());
        variant_clauses.insert(variant_clauses.end(), std::make_move_iterator(clauses.begin()),
                               std::make_move_iterator(clauses.end()));
        return std::make_tuple(variant_clauses, rest);
    }

    return parse_clause<UntypedClauseType>(it, end, TypeList<Tails...>{});
}

template <typename SupportedResultParsers, typename SupportedClauseParsers, typename UntypedReferenceType,
          typename UntypedClauseType, std::enable_if_t<are_parsers_v<SupportedResultParsers>, bool>>
auto build_untyped_query(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedReferenceType, std::vector<UntypedClauseType>>> {
    const auto maybe_reference = parse_result_cl<UntypedReferenceType>(it, end, SupportedResultParsers{});
    if (!maybe_reference.has_value()) {
        return std::nullopt;
    }

    const auto& [reference, rest] = maybe_reference.value();
    it = rest;

    auto clauses = std::vector<UntypedClauseType>{};
    while (it != end) {
        const auto maybe_clause = parse_clause<UntypedClauseType>(it, end, SupportedClauseParsers{});
        if (!maybe_clause.has_value()) {
            break;
        }

        const auto& [created_clauses, rest] = maybe_clause.value();
        clauses.insert(clauses.end(), created_clauses.begin(), created_clauses.end());
        it = rest;
    }

    if (it == end) {
        return std::make_tuple(reference, clauses);
    }
    return std::nullopt;
}
} // namespace qps::untyped::detail