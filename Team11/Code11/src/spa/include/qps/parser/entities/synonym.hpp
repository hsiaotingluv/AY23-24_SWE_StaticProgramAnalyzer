#pragma once

#include "qps/parser/entities/primitives.hpp"

#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>

namespace qps {

struct Integer {
    uint32_t value;

    friend auto operator<<(std::ostream& os, const Integer& integer) -> std::ostream& {
        os << integer.value;
        return os;
    }

    friend auto operator==(const Integer& lhs, const Integer& rhs) -> bool {
        return lhs.value == rhs.value;
    }

    friend auto operator==(const Integer& lhs, const int& rhs) -> bool {
        return lhs.value == rhs;
    }

    friend auto operator==(const int& lhs, const Integer& rhs) -> bool {
        return lhs == rhs.value;
    }
};

// StmtSynonym := RawStmtSynonym | ReadSynonym | PrintSynonym | CallSynonym | WhileSynonym | IfSynonym | AssignSynonym
struct RawStmtSynonym {
    static constexpr auto keyword = "stmt";
    IDENT name;
};

struct ReadSynonym {
    static constexpr auto keyword = "read";
    IDENT name;
};

struct PrintSynonym {
    static constexpr auto keyword = "print";
    IDENT name;
};

struct CallSynonym {
    static constexpr auto keyword = "call";
    IDENT name;
};

struct WhileSynonym {
    static constexpr auto keyword = "while";
    IDENT name;
};

struct IfSynonym {
    static constexpr auto keyword = "if";
    IDENT name;
};

struct AssignSynonym {
    static constexpr auto keyword = "assign";
    IDENT name;
};

using StmtSynonym =
    std::variant<RawStmtSynonym, ReadSynonym, PrintSynonym, CallSynonym, WhileSynonym, IfSynonym, AssignSynonym>;

struct VarSynonym {
    static constexpr auto keyword = "variable";
    IDENT name;
};

struct ConstSynonym {
    static constexpr auto keyword = "constant";
    IDENT name;
};

struct ProcSynonym {
    static constexpr auto keyword = "procedure";
    IDENT name;
};

using Synonym = std::variant<RawStmtSynonym, ReadSynonym, PrintSynonym, CallSynonym, WhileSynonym, IfSynonym,
                             AssignSynonym, VarSynonym, ConstSynonym, ProcSynonym>;

template <class T, class U>
struct is_one_of;

template <class T, class... Ts>
struct is_one_of<T, std::variant<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

inline auto operator==(const Synonym& lhs, const Synonym& rhs) -> bool {
    return std::visit(
        [](auto&& lhs, auto&& rhs) {
            using TL = std::decay_t<decltype(lhs)>;
            using TR = std::decay_t<decltype(rhs)>;
            if constexpr (std::is_same_v<TL, TR>) {
                return lhs.name == rhs.name;
            } else {
                return false;
            }
        },
        lhs, rhs);
}

inline auto operator<<(std::ostream& os, const RawStmtSynonym& stmt_syn) -> std::ostream& {
    os << "RawStmtSynonym(" << stmt_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const ReadSynonym& read_syn) -> std::ostream& {
    os << "ReadSynonym(" << read_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const PrintSynonym& print_syn) -> std::ostream& {
    os << "PrintSynonym(" << print_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const CallSynonym& call_syn) -> std::ostream& {
    os << "CallSynonym(" << call_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const WhileSynonym& while_syn) -> std::ostream& {
    os << "WhileSynonym(" << while_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const IfSynonym& if_syn) -> std::ostream& {
    os << "IfSynonym(" << if_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const AssignSynonym& assign_syn) -> std::ostream& {
    os << "AssignSynonym(" << assign_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const VarSynonym& var_syn) -> std::ostream& {
    os << "VarSynonym(" << var_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const ConstSynonym& const_syn) -> std::ostream& {
    os << "ConstSynonym(" << const_syn.name << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const ProcSynonym& proc_syn) -> std::ostream& {
    os << "ProcSynonym(" << proc_syn.name << ")";
    return os;
}

template <typename... Ts, std::enable_if_t<(sizeof...(Ts) > 0), int> = 0>
auto operator<<(std::ostream& os, const std::variant<Ts...> stmt) -> std::ostream& {
    return std::visit(
        [&os](auto&& x) -> std::ostream& {
            os << x;
            return os;
        },
        stmt);
}

using StmtRef = std::variant<WildCard, StmtSynonym, Integer>;
using EntRef = std::variant<WildCard, Synonym, QuotedIdent>;

using Synonyms = std::vector<Synonym>;

// Helper functions
template <typename T>
auto is_synonym(const T& syn) -> bool {
    return std::holds_alternative<Synonym>(syn);
}

auto is_var_syn(const Synonym& syn) -> bool;
auto is_var_syn(const EntRef& ent_ref) -> bool;
auto is_proc_syn(const Synonym& syn) -> bool;
auto is_proc_syn(const EntRef& ent_ref) -> bool;
auto is_assign_syn(const Synonym& syn) -> bool;

auto is_stmt_synonym(Synonym synonym) -> bool;
auto get_stmt_synonym(Synonym synonym) -> std::optional<StmtSynonym>;

auto find_syn(const Synonyms& declared_synonyms, std::string syn_name) -> std::optional<Synonym>;
auto find_stmt_syn(const Synonyms& declared_synonyms, std::string syn_name) -> std::optional<StmtSynonym>;

} // namespace qps
