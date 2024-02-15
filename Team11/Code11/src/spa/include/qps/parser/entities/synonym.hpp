#pragma once

#include "qps/parser/entities/primitives.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <ostream>
#include <type_traits>
#include <variant>
#include <vector>

namespace qps {

class Synonym {
    IDENT name;

  public:
    virtual ~Synonym() = default;

    explicit Synonym(IDENT name) : name(std::move(name)) {
    }

    explicit Synonym(std::string name) : name(std::move(name)) {
    }

    [[nodiscard]] virtual auto get_keyword() const -> std::string = 0;

    [[nodiscard]] auto is_keyword_match(const std::string& str) const -> bool {
        return str == get_keyword();
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }

    auto operator==(const Synonym& rhs) const -> bool {
        // I'm aware typeid may incur some performance cost, but will stick to it for now
        return typeid(*this) == typeid(rhs) && name == rhs.name;
    }

    friend auto operator<<(std::ostream& os, const Synonym& synonym) -> std::ostream& {
        return os << synonym.get_keyword() << "(" << synonym.name << ")";
    }
};

// StmtSynonym := AnyStmtSynonym | ReadSynonym | PrintSynonym | CallSynonym | WhileSynonym | IfSynonym |
// AssignSynonym
class StmtSynonym : public Synonym {
  public:
    using Synonym::Synonym;
};

class AnyStmtSynonym : public StmtSynonym {
  public:
    static constexpr auto keyword = "stmt";

    explicit AnyStmtSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit AnyStmtSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class ReadSynonym : public StmtSynonym {
  public:
    static constexpr auto keyword = "read";

    explicit ReadSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit ReadSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class PrintSynonym : public StmtSynonym {
  public:
    static constexpr auto keyword = "print";

    explicit PrintSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit PrintSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class CallSynonym : public StmtSynonym {
  public:
    static constexpr auto keyword = "call";

    explicit CallSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit CallSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class WhileSynonym : public StmtSynonym {
  public:
    static constexpr auto keyword = "while";

    explicit WhileSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit WhileSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class IfSynonym : public StmtSynonym {
  public:
    static constexpr auto keyword = "if";

    explicit IfSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit IfSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class AssignSynonym : public StmtSynonym {
  public:
    static constexpr auto keyword = "assign";

    explicit AssignSynonym(IDENT name) : StmtSynonym(std::move(name)) {
    }

    explicit AssignSynonym(std::string name) : StmtSynonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class VarSynonym : public Synonym {
  public:
    static constexpr auto keyword = "variable";

    explicit VarSynonym(IDENT name) : Synonym(std::move(name)) {
    }

    explicit VarSynonym(std::string name) : Synonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class ConstSynonym : public Synonym {
  public:
    static constexpr auto keyword = "constant";

    explicit ConstSynonym(IDENT name) : Synonym(std::move(name)) {
    }

    explicit ConstSynonym(std::string name) : Synonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

class ProcSynonym : public Synonym {
  public:
    static constexpr auto keyword = "procedure";

    explicit ProcSynonym(IDENT name) : Synonym(std::move(name)) {
    }

    explicit ProcSynonym(std::string name) : Synonym(std::move(name)) {
    }

    [[nodiscard]] auto get_keyword() const -> std::string override {
        return keyword;
    }
};

inline auto operator<<(std::ostream& os, const AnyStmtSynonym& stmt_syn) -> std::ostream& {
    os << "AnyStmtSynonym(" << stmt_syn.get_name() << ")";
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
inline auto operator<<(std::ostream& os, const std::variant<Ts...>& stmt) -> std::ostream& {
    return std::visit(
        [&os](auto&& x) -> std::ostream& {
            return os << x;
        },
        stmt);
}

using StmtRef = std::variant<WildCard, std::shared_ptr<StmtSynonym>, Integer>;
using EntRef = std::variant<WildCard, std::shared_ptr<Synonym>, QuotedIdent>;

template <>
inline auto operator<<(std::ostream& os, const EntRef& ent_ref) -> std::ostream& {
    return std::visit(overloaded{
                          [&os](const std::shared_ptr<Synonym>& syn) -> std::ostream& {
                              return os << *syn;
                          },
                          [&os](auto&& x) -> std::ostream& {
                              return os << x;
                          },
                      },
                      ent_ref);
}

template <>
inline auto operator<<(std::ostream& os, const StmtRef& ent_ref) -> std::ostream& {
    return std::visit(overloaded{
                          [&os](const std::shared_ptr<StmtSynonym>& stmt_syn) -> std::ostream& {
                              auto syn = std::static_pointer_cast<Synonym>(stmt_syn);
                              return os << *syn;
                          },
                          [&os](auto&& x) -> std::ostream& {
                              return os << x;
                          },
                      },
                      ent_ref);
}

using Synonyms = std::vector<std::shared_ptr<Synonym>>;

// // Helper functions
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

template <typename T>
inline auto operator==(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) -> bool {
    if (!lhs && !rhs) {
        return true;
    }
    if (!lhs || !rhs) {
        return false;
    }
    return *lhs == *rhs;
}

// template <typename T>
// auto is_synonym(const T& syn) -> bool {
//     return std::holds_alternative<Synonym>(syn);
// }

auto is_var_syn(const std::shared_ptr<Synonym>& syn) -> bool;
// auto is_var_syn(const EntRef& ent_ref) -> bool;
// auto is_proc_syn(const Synonym& syn) -> bool;
// auto is_proc_syn(const EntRef& ent_ref) -> bool;
// auto is_assign_syn(const Synonym& syn) -> bool;

// auto is_stmt_synonym(Synonym synonym) -> bool;
// auto get_stmt_synonym(Synonym synonym) -> std::optional<StmtSynonym>;

} // namespace qps

namespace std {
template <>
struct hash<qps::Synonym> {
    auto operator()(const qps::Synonym& syn) const -> size_t {
        return hash<std::string>{}(syn.get_keyword() + syn.get_name().get_value());
    }
};

} // namespace std