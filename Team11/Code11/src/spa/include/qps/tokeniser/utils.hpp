#pragma once

#include "qps/template_utils.hpp"

template <int Index, template <typename> typename Tokeniser, typename T>
inline void push_array_rec(T&, qps::TypeList<>) {
    return;
}

template <int Index, template <typename> typename Tokeniser, typename T, typename Head, typename... Tails>
void push_array_rec(T& vector, qps::TypeList<Head, Tails...>) {
    vector.at(Index) = std::make_shared<Tokeniser<Head>>();
    push_array_rec<Index + 1, Tokeniser>(vector, qps::TypeList<Tails...>{});
}