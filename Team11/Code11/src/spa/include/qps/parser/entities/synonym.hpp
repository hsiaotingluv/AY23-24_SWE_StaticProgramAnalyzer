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

// StmtSynonym := UntypedStmtSynonym | ReadSynonym | PrintSynonym | CallSynonym | WhileSynonym | IfSynonym | AssignSynonym
class UntypedStmtSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "stmt";

    explicit UntypedStmtSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit UntypedStmtSynonym(std::string name) : name(std::move(name)) {
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }

    auto operator==(const UntypedStmtSynonym& rhs) const -> bool {
        return name == rhs.name;
    }
};

class ReadSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "read";

    explicit ReadSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit ReadSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const ReadSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

class PrintSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "print";

    explicit PrintSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit PrintSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const PrintSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

class CallSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "call";

    explicit CallSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit CallSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const CallSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

class WhileSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "while";

    explicit WhileSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit WhileSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const WhileSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

class IfSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "if";

    explicit IfSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit IfSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const IfSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

class AssignSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "assign";

    explicit AssignSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit AssignSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const AssignSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

using StmtSynonym =
    std::variant<UntypedStmtSynonym, ReadSynonym, PrintSynonym, CallSynonym, WhileSynonym, IfSynonym, AssignSynonym>;

class VarSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "variable";

    explicit VarSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit VarSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const VarSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

class ConstSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "constant";

    explicit ConstSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit ConstSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const ConstSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

class ProcSynonym {
    IDENT name;

  public:
    static constexpr auto keyword = "procedure";

    explicit ProcSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit ProcSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const ProcSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }
};

using Synonym = std::variant<UntypedStmtSynonym, ReadSynonym, PrintSynonym, CallSynonym, WhileSynonym, IfSynonym,
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
                return lhs.get_name() == rhs.get_name();
            } else {
                return false;
            }
        },
        lhs, rhs);
}

inline auto operator==(const StmtSynonym& lhs, const StmtSynonym& rhs) -> bool {
    return std::visit(
        [](auto&& lhs, auto&& rhs) {
            using TL = std::decay_t<decltype(lhs)>;
            using TR = std::decay_t<decltype(rhs)>;
            if constexpr (std::is_same_v<TL, TR>) {
                return lhs.get_name() == rhs.get_name();
            } else {
                return false;
            }
        },
        lhs, rhs);
}

inline auto operator<<(std::ostream& os, const UntypedStmtSynonym& stmt_syn) -> std::ostream& {
    os << "UntypedStmtSynonym(" << stmt_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const ReadSynonym& read_syn) -> std::ostream& {
    os << "ReadSynonym(" << read_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const PrintSynonym& print_syn) -> std::ostream& {
    os << "PrintSynonym(" << print_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const CallSynonym& call_syn) -> std::ostream& {
    os << "CallSynonym(" << call_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const WhileSynonym& while_syn) -> std::ostream& {
    os << "WhileSynonym(" << while_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const IfSynonym& if_syn) -> std::ostream& {
    os << "IfSynonym(" << if_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const AssignSynonym& assign_syn) -> std::ostream& {
    os << "AssignSynonym(" << assign_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const VarSynonym& var_syn) -> std::ostream& {
    os << "VarSynonym(" << var_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const ConstSynonym& const_syn) -> std::ostream& {
    os << "ConstSynonym(" << const_syn.get_name() << ")";
    return os;
}

inline auto operator<<(std::ostream& os, const ProcSynonym& proc_syn) -> std::ostream& {
    os << "ProcSynonym(" << proc_syn.get_name() << ")";
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
inline auto operator==(const StmtRef& lhs, const StmtRef& rhs) -> bool {
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

namespace std {
template <>
struct hash<qps::UntypedStmtSynonym> {
    auto operator()(const qps::UntypedStmtSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::UntypedStmtSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::ReadSynonym> {
    auto operator()(const qps::ReadSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::ReadSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::PrintSynonym> {
    auto operator()(const qps::PrintSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::PrintSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::CallSynonym> {
    auto operator()(const qps::CallSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::CallSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::WhileSynonym> {
    auto operator()(const qps::WhileSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::WhileSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::IfSynonym> {
    auto operator()(const qps::IfSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::IfSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::AssignSynonym> {
    auto operator()(const qps::AssignSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::AssignSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::VarSynonym> {
    auto operator()(const qps::VarSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::VarSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::ConstSynonym> {
    auto operator()(const qps::ConstSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::ConstSynonym::keyword + syn.get_name().get_value());
    }
};

template <>
struct hash<qps::ProcSynonym> {
    auto operator()(const qps::ProcSynonym& syn) const -> size_t {
        return hash<std::string>{}(qps::ProcSynonym::keyword + syn.get_name().get_value());
    }
};

} // namespace std