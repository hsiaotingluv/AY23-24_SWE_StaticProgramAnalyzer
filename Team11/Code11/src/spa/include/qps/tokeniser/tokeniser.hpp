#pragma once

#include "common/tokeniser/binop.hpp"
#include "common/tokeniser/integer.hpp"
#include "common/tokeniser/junk.hpp"
#include "common/tokeniser/string.hpp"
#include "qps/tokeniser/miscellaneous.hpp"
#include "qps/tokeniser/qps_delimiter.hpp"
#include "qps/tokeniser/wildcard_tokeniser.hpp"

#include <array>
#include <memory>

namespace qps {

class QueryProcessingSystemTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 7>{
        std::make_shared<IntegerTokenizer>(),      std::make_shared<StringTokenizer>(),
        std::make_shared<QPSDelimiterTokenizer>(), std::make_shared<JunkTokenizer>(),
        std::make_shared<BinopTokenizer>(),        std::make_shared<WildCardTokenizer>(),
        std::make_shared<MiscellaneousTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override;
};
} // namespace qps
