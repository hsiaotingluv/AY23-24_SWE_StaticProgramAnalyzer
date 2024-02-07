#pragma once
#include "common/tokeniser/tokenizer.hpp"

namespace qps {
using namespace tokenizer;

class WildCardTokenizer : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput &input) const -> TokeniserOutput override;
};
} // namespace qps