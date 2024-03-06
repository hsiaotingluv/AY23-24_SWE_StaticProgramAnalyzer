#pragma once

#include "common/tokeniser/tokenizer.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/template_utils.hpp"
#include <memory>

namespace tokenizer {
template <typename T>
class RelationshipTokeniser : public Tokenizer {
  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return tokenize_string(input, T::keyword, TokenType::Relationship);
    }
};

template <int Index, typename T>
inline void push_array_rec(T&, qps::TypeList<>) {
    return;
}

template <int Index, typename T, typename Head, typename... Tails>
void push_array_rec(T& vector, qps::TypeList<Head, Tails...>) {
    vector.at(Index) = std::make_shared<RelationshipTokeniser<Head>>();
    push_array_rec<Index + 1>(vector, qps::TypeList<Tails...>{});
}

class QPSRelationshipTokenizer : public Tokenizer {
  private:
    static inline const auto tokenizers = []() {
        auto vector = std::array<std::shared_ptr<Tokenizer>, qps::num_elem_v<qps::RelationshipList>>{};
        push_array_rec<0>(vector, qps::RelationshipList{});
        return vector;
    }();

  public:
    [[nodiscard]] auto tokenize(const TokeniserInput& input) const -> TokeniserOutput override {
        return one_of(input, tokenizers.begin(), tokenizers.end());
    }
};

} // namespace tokenizer
