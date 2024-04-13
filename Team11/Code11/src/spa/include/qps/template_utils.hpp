#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <type_traits>
#include <variant>
#include <vector>

namespace qps {
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename T, typename Variant>
struct is_variant_member {
    static constexpr bool value = false;
};

template <typename T, typename... Ts>
struct is_variant_member<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

template <class T, class U>
struct is_one_of;

template <class T, class... Ts>
struct is_one_of<T, std::variant<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

template <typename T, typename Variant>
inline constexpr bool is_variant_member_v = is_variant_member<T, Variant>::value;

// Wrapper around variadic templates
template <typename... T>
struct TypeList {};

// Check if a type is a TypeList
template <typename T>
struct is_type_list : std::false_type {};

template <typename... T>
struct is_type_list<TypeList<T...>> : std::true_type {};

template <typename T>
inline constexpr bool is_type_list_v = is_type_list<T>::value;

// Concatenate TypeLists
template <typename... Lists>
struct concat;

template <typename... Ts, typename... Us>
struct concat<TypeList<Ts...>, TypeList<Us...>> {
    using type = TypeList<Ts..., Us...>;
};

template <typename T, typename U>
using concat_t = typename concat<T, U>::type;

// Count number of elements in a TypeList
template <typename T>
struct num_elem;

template <>
struct num_elem<qps::TypeList<>> {
    static constexpr int value = 0;
};

template <typename Head, typename... Tails>
struct num_elem<qps::TypeList<Head, Tails...>> {

    static constexpr int value = 1 + num_elem<qps::TypeList<Tails...>>::value;
};

template <typename T>
static constexpr int num_elem_v = num_elem<T>::value;

// Check if a TypeList is empty
template <typename T>
struct is_type_list_empty : std::false_type {};

template <>
struct is_type_list_empty<TypeList<>> : std::true_type {};

template <typename T>
static constexpr bool is_typelist_empty_v = is_type_list_empty<T>::value;

// Check if a TypeList contains a type
template <typename T, typename U>
struct is_member_of;

template <typename Elem>
struct is_member_of<Elem, TypeList<>> {
    static constexpr bool value = false;
};

template <typename Elem, typename Head, typename... Tails>
struct is_member_of<Elem, TypeList<Head, Tails...>> {
    static constexpr bool value = std::is_same_v<Elem, Head> || is_member_of<Elem, TypeList<Tails...>>::value;
};

template <typename Elem, typename List>
static constexpr bool is_member_v = is_member_of<Elem, List>::value;

// Check if all elements of a TypeList satisfy a condition
template <typename T, template <typename> typename Func>
struct all_of;

template <template <typename> typename Func>
struct all_of<TypeList<>, Func> {
    static constexpr bool value = true;
};

template <template <typename> typename Func, typename Head, typename... Tails>
struct all_of<TypeList<Head, Tails...>, Func> {
    static constexpr bool value = Func<Head>::value && all_of<TypeList<Tails...>, Func>::value;
};

template <typename T, template <typename> typename Func>
static constexpr bool all_of_v = all_of<T, Func>::value;

template <typename TypeList>
struct type_list_to_variant;

template <typename... Types>
struct type_list_to_variant<TypeList<Types...>> {
    using type = std::variant<Types...>;
};

template <typename T>
using type_list_to_variant_t = typename type_list_to_variant<T>::type;

template <typename... Ts, typename = std::enable_if_t<(sizeof...(Ts) > 0)>>
auto operator<<(std::ostream& os, const std::variant<Ts...>& some_variant) -> std::ostream& {
    return std::visit(
        [&os](auto&& x) -> std::ostream& {
            return os << x;
        },
        some_variant);
}

template <typename... Ts>
auto operator<<(std::ostream& os, const std::vector<std::variant<Ts...>>& some_variants) -> std::ostream& {
    for (const auto& variant : some_variants) {
        os << variant << "\n";
    }
    return os;
}

template <typename T>
auto operator==(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) -> bool {
    if (!lhs && !rhs) {
        return true;
    }
    if (!lhs || !rhs) {
        return false;
    }
    return *lhs == *rhs;
}

template <typename T>
auto operator<(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) -> bool {
    if (lhs == nullptr) {
        return rhs != nullptr;
    }
    if (rhs == nullptr) {
        return false;
    }
    return *lhs < *rhs;
}

template <typename T>
auto operator==(const std::variant<T>& lhs, const std::variant<T>& rhs) -> bool {
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

template <typename... T>
struct get_return_type;

template <>
struct get_return_type<TypeList<>> {
    using type = TypeList<>;
};

template <typename Head, typename... Tails>
struct get_return_type<TypeList<Head, Tails...>> {
    using type = concat_t<TypeList<typename Head::ClauseType>, typename get_return_type<TypeList<Tails...>>::type>;
};

template <typename T>
using get_return_type_t = typename get_return_type<T>::type;

} // namespace qps