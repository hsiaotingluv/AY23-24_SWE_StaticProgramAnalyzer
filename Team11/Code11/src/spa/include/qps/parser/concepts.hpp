#pragma once

#include "qps/parser/parser_helper.hpp"
#include "qps/template_utils.hpp"

#include <string>
#include <type_traits>
#include <vector>

namespace qps {

// Concept: lexable
template <typename T>
struct is_lexable : std::true_type {
    // Ensure that T exposes a keyword
    static_assert(T::keyword, "T must have a keyword");

    // Ensure that T::keyword is a string
    static_assert(std::is_convertible_v<decltype(T::keyword), std::string>,
                  "T::keyword must be convertible to std::string");
};

template <typename T>
constexpr bool is_lexable_v = is_lexable<T>::value;

// Concept: parseable
// A type is parseable if it is lexable
template <typename T>
struct is_parseable : is_lexable<T> {};

template <typename T>
constexpr bool is_parseable_v = is_parseable<T>::value;

// Concept: parseables
// An empty TypeList, or a TypeList of parseables
template <typename T>
struct are_parseables : std::true_type {
    // Ensure that T is a TypeList
    static_assert(is_type_list_v<T>, "T must be a TypeList");

    // Ensure that all elements of T are parseables
    static_assert(all_of_v<T, is_parseable>, "All elements of T must be parseables");
};

template <typename T>
constexpr bool are_parseables_v = are_parseables<T>::value;

// Concept: parser
template <typename T>
struct is_parser : std::true_type {
    // Ensure that T exposes a ClauseType
    using ReturnType = typename T::ClauseType;
    static_assert(std::is_same_v<ReturnType, ReturnType>, "T must expose return type as a ClauseType");

    // Ensure that T::parse is a valid function
    static_assert(std::is_invocable_v<decltype(&T::parse), std::vector<Token>::const_iterator,
                                      const std::vector<Token>::const_iterator&>,
                  "T must expose a parse function with signature: "
                  "std::optional<std::tuple<ReturnType, std::vector<Token>::const_iterator>>");
};

template <typename T>
constexpr bool is_parser_v = is_parser<T>::value;

// Concept: parsers
// An empty TypeList, or a TypeList of parsers
template <typename T>
struct are_parsers : std::true_type {
    // Ensure that T is a TypeList
    static_assert(is_type_list_v<T>, "T must be a TypeList");

    // Ensure that all elements of T are parsers
    static_assert(all_of_v<T, is_parser>, "All elements of T must be parsers");
};

template <typename T>
constexpr bool are_parsers_v = are_parsers<T>::value;

// Concept: parser_strategy
template <typename Strategy>
struct is_parser_strategy : std::true_type {
    using KeywordsArray = decltype(Strategy::keywords);

    // Ensure that T::keywords is a std::array
    static constexpr auto N = std::tuple_size_v<KeywordsArray>;
    static_assert(std::is_same_v<std::remove_cv_t<KeywordsArray>, std::array<std::string_view, N>>,
                  "T::keywords must be a std::array<std::string_view, N>");

    // Ensure that T exposes a ClauseType
    using ReturnType = typename Strategy::ClauseType;
    static_assert(std::is_same_v<ReturnType, ReturnType>, "T must expose return type as a ClauseType");

    // Ensure that T::parse is a valid function
    static_assert(std::is_invocable_v<decltype(&Strategy::parse_clause), std::vector<Token>::const_iterator,
                                      const std::vector<Token>::const_iterator&>,
                  "T must expose a parse_clause function with signature: "
                  "std::optional<std::tuple<ReturnType, std::vector<Token>::const_iterator>>");
};

template <typename Strategy>
constexpr bool is_parser_strategy_v = is_parser_strategy<Strategy>::value;

// Concept: parser_strategies
// An empty TypeList, or a TypeList of parser strategies
template <typename T>
struct are_parser_strategies : std::true_type {
    // Ensure that T is a TypeList
    static_assert(is_type_list_v<T>, "T must be a TypeList");

    // Ensure that all elements of T are parser strategies
    static_assert(all_of_v<T, is_parser_strategy>, "All elements of T must be parser strategies");
};

} // namespace qps