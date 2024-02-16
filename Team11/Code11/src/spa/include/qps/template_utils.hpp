#pragma once

#include <ostream>
#include <variant>

namespace qps {
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename T, typename Variant>
struct is_variant_member;

template <typename T, typename... Ts>
struct is_variant_member<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

template <class T, class U>
struct is_one_of;

template <class T, class... Ts>
struct is_one_of<T, std::variant<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

template <typename T, typename Variant>
inline constexpr bool is_variant_member_v = is_variant_member<T, Variant>::value;

template <typename... T>
struct TypeList {};

template <typename... Lists>
struct Concat;

template <typename... Ts, typename... Us>
struct Concat<TypeList<Ts...>, TypeList<Us...>> {
    using type = TypeList<Ts..., Us...>;
};

template <typename TypeList>
struct TypeListToVariant;

template <typename... Types>
struct TypeListToVariant<TypeList<Types...>> {
    using type = std::variant<Types...>;
};

template <typename... Ts, std::enable_if_t<(sizeof...(Ts) > 0), int> = 0>
inline auto operator<<(std::ostream& os, const std::variant<Ts...>& some_variant) -> std::ostream& {
    return std::visit(
        [&os](auto&& x) -> std::ostream& {
            return os << x;
        },
        some_variant);
}

template <typename T>
inline auto operator==(const std::variant<T>& lhs, const std::variant<T>& rhs) -> bool {
    return std::visit(
        [](auto&& lhs, auto&& rhs) {
            using TL = std::decay_t<decltype(lhs)>;
            using TR = std::decay_t<decltype(rhs)>;
            if constexpr (std::is_same_v<TL, TR>) {
                return lhs == rhs;
            } else {
                return false;
            }
        },
        lhs, rhs);
}

} // namespace qps