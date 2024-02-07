#pragma once

#include "common/tokeniser/binop.hpp"
#include "common/tokeniser/integer.hpp"
#include "common/tokeniser/junk.hpp"
#include "common/tokeniser/string.hpp"
#include "common/tokeniser/tokenizer.hpp"
#include "sp/tokeniser/delimiter.hpp"
#include "sp/tokeniser/logical.hpp"
#include "sp/tokeniser/relational.hpp"

namespace sp {
using namespace tokenizer;

class SourceProcessorTokenizer : public Tokenizer {
  private:
    /*
     * Note: The order of the tokenizer defined is important, because it determines
     * How the token is going to be parsed, for e.g. == => should be DoubleEqual
     * instead of 2 Assign Tokens, similarly there are >= vs >, etc.
     */
    static inline const auto tokenizers = std::array<std::shared_ptr<Tokenizer>, 7>{
        std::make_shared<IntegerTokenizer>(),     std::make_shared<StringTokenizer>(),
        std::make_shared<SPDelimiterTokenizer>(), std::make_shared<JunkTokenizer>(),
        std::make_shared<BinopTokenizer>(),       std::make_shared<SPRelationalTokenizer>(),
        std::make_shared<SPLogicalTokenizer>()};

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};
} // namespace sp
