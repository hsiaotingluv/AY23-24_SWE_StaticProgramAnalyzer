#pragma once

#include "common/tokeniser/tokenizer.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/template_utils.hpp"

#include "utils.hpp"
#include <memory>

namespace tokenizer {
template <typename T>
class RelationshipTokeniser : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, T::keyword, TokenType::Relationship);
    }
};

class QPSRelationshipTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = []() {
        auto vector = std::array<std::shared_ptr<Tokenizer>, qps::num_elem_v<qps::RelationshipList>>{};
        push_array_rec<0, RelationshipTokeniser>(vector, qps::RelationshipList{});
        return vector;
    }();

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};

} // namespace tokenizer
