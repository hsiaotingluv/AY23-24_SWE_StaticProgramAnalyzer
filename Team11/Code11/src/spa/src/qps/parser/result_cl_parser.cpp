#include "qps/parser/untyped/result_cl_parser.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"

namespace qps::untyped {
auto SelectSynonymParser::parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedSynonym, std::vector<Token>::const_iterator>> {
    constexpr auto EXPECTED_LENGTH = 2;

    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    const auto maybe_it = detail::parse_keywords(keywords, it, end);
    if (!maybe_it.has_value()) {
        return std::nullopt;
    }
    it = maybe_it.value();

    return detail::parse_synonym(it, end);
}

auto SelectSynonymsParser::parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<std::vector<UntypedSynonym>, std::vector<Token>::const_iterator>> {

    std::vector<UntypedSynonym> synonyms;
    const auto maybe_it = detail::parse_keywords(keywords, it, end);
    if (!maybe_it.has_value()) {
        return std::nullopt;
    }
    it = maybe_it.value();

    // Consume '<'
    if (it == end || it->T != tokenizer::TokenType::LessThan) {
        return std::nullopt;
    }
    it = std::next(it);

    auto first_synonym = true;
    while (it != end && it->T != tokenizer::TokenType::GreaterThan) {
        if (!first_synonym) {
            // Consume ','
            if (it == end || it->T != tokenizer::TokenType::Comma) {
                return std::nullopt;
            }
            it = std::next(it);
        }

        const auto maybe_synonym = detail::parse_synonym(it, end);
        if (!maybe_synonym.has_value()) {
            break;
        }
        const auto& [synonym, rest] = maybe_synonym.value();
        synonyms.push_back(synonym);
        it = rest;
        first_synonym = false;
    }

    // Consume '>'
    if (it == end || it->T != tokenizer::TokenType::GreaterThan) {
        return std::nullopt;
    }

    return synonyms.empty() ? std::nullopt : std::make_optional(std::make_tuple(synonyms, std::next(it)));
}
} // namespace qps::untyped