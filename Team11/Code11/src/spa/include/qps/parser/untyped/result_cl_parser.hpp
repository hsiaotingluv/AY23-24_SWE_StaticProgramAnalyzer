#pragma once
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/attribute.hpp"
#include "qps/parser/untyped/entities/boolean.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"
#include "qps/template_utils.hpp"

#include <array>
#include <optional>

namespace qps::untyped::detail {
template <typename UntypedReference, typename Head, typename... Tails>
auto parse_one(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
               TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>>;

template <typename UntypedReference, typename Head, typename... Tails>
auto parse_many(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>>;
} // namespace qps::untyped::detail

namespace qps::untyped {
class SelectSynonymStrategy {
  public:
    using ClauseType = UntypedSynonym;

    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

class SelectAttrRefStrategy {
  public:
    using ClauseType = UntypedAttrRef;

    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

template <typename Strategies>
class SelectElemParser {
    static constexpr auto keywords = std::array<std::string_view, 1>{"Select"};

  public:
    using ClauseType = std::vector<type_list_to_variant_t<get_return_type_t<Strategies>>>;

    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>> {
        const auto maybe_it = detail::parse_keywords(keywords, it, end);

        if (!maybe_it.has_value()) {
            return std::nullopt;
        }
        it = maybe_it.value();

        auto maybe_one = detail::parse_one<ClauseType>(it, end, Strategies{});
        if (maybe_one.has_value()) {
            return maybe_one;
        }

        auto maybe_many = detail::parse_many<ClauseType>(it, end, Strategies{});
        if (maybe_many.has_value()) {
            return maybe_many;
        }
        return std::nullopt;
    }
};

using DefaultSelectElemParser = SelectElemParser<TypeList<SelectAttrRefStrategy, SelectSynonymStrategy>>;

class SelectBooleanParser {
    static constexpr auto keywords = std::array<std::string_view, 1>{"Select"};

  public:
    using ClauseType = UntypedBoolean;

    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

using DefaultSupportedSelectParsers = TypeList<SelectBooleanParser, DefaultSelectElemParser>;
} // namespace qps::untyped

namespace qps::untyped::detail {
template <typename UntypedReference>
auto parse_one(std::vector<Token>::const_iterator, const std::vector<Token>::const_iterator&, TypeList<>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>> {
    return std::nullopt;
}

template <typename UntypedReference, typename Head, typename... Tails>
auto parse_one(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
               TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>> {
    const auto maybe_select = Head::parse(it, end);
    if (maybe_select.has_value()) {
        const auto& [reference, rest] = maybe_select.value();
        return std::make_tuple(UntypedReference{reference}, rest);
    }
    return parse_one<UntypedReference>(it, end, TypeList<Tails...>{});
}

template <typename UntypedReference, typename Head, typename... Tails>
auto parse_many(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end,
                TypeList<Head, Tails...>)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>> {
    // Expect '<'
    if (it == end || it->T != tokenizer::TokenType::LessThan) {
        return std::nullopt;
    }
    it = std::next(it);

    // Repeatedly parse once
    auto references = UntypedReference{};
    auto first_clause = true;
    while (it != end) {
        if (!first_clause) {
            // Expect ','
            if (it == end || it->T != tokenizer::TokenType::Comma) {
                break;
            }
            it = std::next(it);
        }

        const auto maybe_reference = parse_one<UntypedReference>(it, end, TypeList<Head, Tails...>{});
        if (!maybe_reference.has_value()) {
            return std::nullopt;
        }
        auto& [reference, rest] = maybe_reference.value();
        references.insert(references.end(), reference.begin(), reference.end());
        it = rest;
        first_clause = false;
    }

    // Expect '>'
    if (it == end || it->T != tokenizer::TokenType::GreaterThan) {
        return std::nullopt;
    }

    return std::make_tuple(references, std::next(it));
}
} // namespace qps::untyped::detail