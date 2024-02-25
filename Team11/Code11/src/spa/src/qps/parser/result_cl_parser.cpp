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
} // namespace qps::untyped