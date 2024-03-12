#pragma once

#include "qps/template_utils.hpp"
#include <array>
#include <type_traits>

namespace qps {

struct ProcName {
    static constexpr auto keyword = "procName";
};

struct VarName {
    static constexpr auto keyword = "varName";
};

struct Value {
    static constexpr auto keyword = "value";
};

struct StmtNum {
    static constexpr auto keyword = "stmt#";
};

using AttrNameList = TypeList<ProcName, VarName, Value, StmtNum>;
using AttrName = type_list_to_variant_t<AttrNameList>;

template <typename T, std::enable_if_t<is_variant_member_v<T, AttrName>, int> = 0>
auto operator==(const T&, const T&) -> bool {
    return true;
}

template <typename T, typename S, std::enable_if_t<is_variant_member_v<T, AttrName>, int> = 0,
          std::enable_if_t<is_variant_member_v<S, AttrName>, int> = 0, std::enable_if_t<!std::is_same_v<T, S>, int> = 0>
auto operator==(const T&, const S&) -> bool {
    return false;
}

template <unsigned long N, typename T>
auto operator<<(std::ostream& os, const std::array<T, N>& keywords) -> std::ostream& {
    for (const auto& keyword : keywords) {
        os << keyword;
    }
    return os;
}

template <typename T, std::enable_if_t<is_variant_member_v<T, AttrName>, int> = 0>
auto operator<<(std::ostream& os, const T&) -> std::ostream& {
    os << T::keyword;
    return os;
}

} // namespace qps