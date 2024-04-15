#pragma once

#include "common/tokeniser/tokenizer.hpp"
#include "qps/parser/concepts.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/template_utils.hpp"

#include "utils.hpp"
#include <memory>

namespace tokenizer {
template <typename T>
class RelationshipTokeniser final : public Tokenizer {
    static_assert(qps::is_lexable_v<T>, "T must be lexable");

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, T::keyword, TokenType::String);
    }
};

class QPSRelationshipTokenizer final : public Tokenizer {
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
