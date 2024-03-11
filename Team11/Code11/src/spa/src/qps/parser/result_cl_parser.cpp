#include "qps/parser/untyped/result_cl_parser.hpp"
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/untyped_parser_helper.hpp"

namespace qps::untyped {
auto SelectSynonymStrategy::parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedSynonym, std::vector<Token>::const_iterator>> {
    return detail::parse_synonym(it, end);
}

auto SelectAttrRefStrategy::parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedAttrRef, std::vector<Token>::const_iterator>> {
    return detail::parse_attr_ref(it, end);
}

auto SelectBooleanParser::parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedBoolean, std::vector<Token>::const_iterator>> {
    constexpr auto EXPECTED_LENGTH = 2;
    constexpr auto BOOLEAN_STRING = "BOOLEAN";

    if (std::distance(it, end) < EXPECTED_LENGTH) {
        return std::nullopt;
    }

    const auto maybe_it = detail::parse_keywords(keywords, it, end);
    if (!maybe_it.has_value()) {
        return std::nullopt;
    }
    it = maybe_it.value();

    if (it == end) {
        return std::nullopt;
    }

    const auto maybe_boolean = *it;
    if (!is_string(maybe_boolean) || maybe_boolean.content != BOOLEAN_STRING) {
        return std::nullopt;
    }

    return std::make_optional(std::make_tuple(UntypedBoolean{}, std::next(it)));
}
} // namespace qps::untyped