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

class Synonym {
    IDENT name;

public:
    explicit Synonym(IDENT name) : name(std::move(name)) {
    }

    explicit Synonym(std::string name) : name(std::move(name)) {
    }

    [[nodiscard]] virtual auto get_keyword() const -> std::string = 0;

    virtual auto is_keyword_match(const std::string &str) -> bool {
        return str == get_keyword();
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }

    auto operator==(const Synonym& rhs) const -> bool {
        // I'm aware typeid may incur some performance cost, but will stick to it for now
        return typeid(*this) == typeid(rhs) && name == rhs.name;
    }

};

// StmtSynonym := RawStmtSynonym | ReadSynonym | PrintSynonym | CallSynonym | WhileSynonym | IfSynonym | AssignSynonym
class StmtSynonym : public Synonym {
public:
    explicit StmtSynonym(IDENT name) : Synonym(std::move(name)){
    }

    explicit StmtSynonym(std::string name) : Synonym(std::move(name)){
    }
};

class RawStmtSynonym : public StmtSynonym {
    static constexpr auto keyword = "stmt";

public:
    explicit RawStmtSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit RawStmtSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class ReadSynonym : public StmtSynonym {
    static constexpr auto keyword = "read";

  public:
    explicit ReadSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit ReadSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class PrintSynonym : public StmtSynonym {
    static constexpr auto keyword = "print";

  public:
    explicit PrintSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit PrintSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class CallSynonym : public StmtSynonym {
    static constexpr auto keyword = "call";

  public:
    explicit CallSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit CallSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class WhileSynonym : public StmtSynonym {
    static constexpr auto keyword = "while";

  public:
    explicit WhileSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit WhileSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class IfSynonym : public StmtSynonym {
    static constexpr auto keyword = "if";

  public:
    explicit IfSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit IfSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class AssignSynonym : public StmtSynonym {
    static constexpr auto keyword = "assign";

  public:
    explicit AssignSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit AssignSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

//using StmtSynonym =
//    std::variant<RawStmtSynonym, ReadSynonym, PrintSynonym, CallSynonym, WhileSynonym, IfSynonym, AssignSynonym>;

class VarSynonym : public Synonym {
    static constexpr auto keyword = "variable";

  public:
    explicit VarSynonym(IDENT name) : Synonym(std::move(name)) {
    }

    explicit VarSynonym(std::string name) : Synonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class ConstSynonym : public Synonym {
    static constexpr auto keyword = "constant";

public:
    explicit ConstSynonym(IDENT name) : Synonym(std::move(name)) {
    }

    explicit ConstSynonym(std::string name) : Synonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class ProcSynonym : public Synonym {
    static constexpr auto keyword = "procedure";

  public:
    explicit ProcSynonym(IDENT name) : Synonym(std::move(name)) {
    }

    explicit ProcSynonym(std::string name) : Synonym(std::move(name)) {
    }

    auto get_keyword() const -> std::string override {
        return keyword;
    }
};

//using Synonym = std::variant<RawStmtSynonym, ReadSynonym, PrintSynonym, CallSynonym, WhileSynonym, IfSynonym,
//                             AssignSynonym, VarSynonym, ConstSynonym, ProcSynonym>;

template <class T, class U>
struct is_one_of;

template <class T, class... Ts>
struct is_one_of<T, std::variant<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

inline auto operator<<(std::ostream& os, const RawStmtSynonym& stmt_syn) -> std::ostream& {
    os << "RawStmtSynonym(" << stmt_syn.get_name() << ")";
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
struct hash<qps::Synonym> {
    auto operator()(const qps::Synonym& syn) const -> size_t {
        return hash<std::string>{}( syn.get_keyword() + syn.get_name().get_value());
    }
};

} // namespace std