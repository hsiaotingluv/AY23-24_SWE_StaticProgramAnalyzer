#pragma once
#include "qps/parser/parser_helper.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <array>

namespace qps::untyped {
class SelectSynonymParser {
    static constexpr auto keywords = std::array<std::string_view, 1>{"Select"};

  public:
    using ClauseType = UntypedSynonym;

    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<ClauseType, std::vector<Token>::const_iterator>>;
};

using DefaultSupportedSelectParsers = TypeList<SelectSynonymParser>;
} // namespace qps::untyped