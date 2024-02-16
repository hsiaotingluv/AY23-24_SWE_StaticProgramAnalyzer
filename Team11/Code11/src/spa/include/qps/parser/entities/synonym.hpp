#pragma once

#include "qps/parser/entities/primitives.hpp"

#include <memory>
#include <ostream>
#include <type_traits>
#include <variant>
#include <vector>

namespace qps {

class Synonym {
    IDENT name;

    [[nodiscard]] virtual auto representation() const -> std::string = 0;

  public:
    virtual ~Synonym() = default;

    explicit Synonym(IDENT name) : name(std::move(name)) {
    }

    explicit Synonym(std::string name) : name(std::move(name)) {
    }

    [[nodiscard]] virtual auto get_keyword() const -> std::string = 0;

    [[nodiscard]] auto is_keyword_match(const std::string& str) const -> bool;

    [[nodiscard]] auto get_name() const -> IDENT;

    auto operator==(const Synonym& rhs) const noexcept -> bool;

    friend auto operator<<(std::ostream& os, const Synonym& synonym) -> std::ostream&;
};

// StmtSynonym := AnyStmtSynonym | ReadSynonym | PrintSynonym | CallSynonym | WhileSynonym | IfSynonym |
// AssignSynonym
class StmtSynonym : public Synonym {
  public:
    using Synonym::Synonym;
};

class AnyStmtSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "stmt";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class ReadSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "read";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class PrintSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "print";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class CallSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "call";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class WhileSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "while";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class IfSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "if";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class AssignSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "assign";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class VarSynonym final : public Synonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "variable";

    using Synonym::Synonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class ConstSynonym final : public Synonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "constant";

    using Synonym::Synonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

class ProcSynonym final : public Synonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "procedure";

    using Synonym::Synonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;
};

using StmtRef = std::variant<WildCard, std::shared_ptr<StmtSynonym>, Integer>;
using EntRef = std::variant<WildCard, std::shared_ptr<Synonym>, QuotedIdent>;

using Synonyms = std::vector<std::shared_ptr<Synonym>>;

// Helper functions
auto is_stmt_synonym(const std::shared_ptr<StmtSynonym>& synonym) -> bool;
auto is_stmt_synonym(const std::shared_ptr<Synonym>& synonym) -> bool;

// Define how to print shared_ptr of Synonym
template <typename T, std::enable_if_t<std::is_base_of_v<Synonym, T>, int> = 0>
inline auto operator<<(std::ostream& os, const std::shared_ptr<T>& ptr) -> std::ostream& {
    if (ptr == nullptr) {
        return os << "Synonym(nullptr)";
    }
    return os << *std::static_pointer_cast<Synonym>(ptr);
}

// Define how to compare shared_ptr of Synonym
template <typename T, std::enable_if_t<std::is_base_of_v<Synonym, T>, int> = 0>
inline auto operator==(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) -> bool {
    if (!lhs && !rhs) {
        return true;
    }
    if (!lhs || !rhs) {
        return false;
    }
    return *lhs == *rhs;
}

} // namespace qps

namespace std {
template <>
struct hash<qps::Synonym> {
    auto operator()(const qps::Synonym& syn) const -> size_t {
        return hash<std::string>{}(syn.get_keyword() + syn.get_name().get_value());
    }
};

template <>
struct hash<std::shared_ptr<qps::Synonym>> {
    auto operator()(const std::shared_ptr<qps::Synonym>& syn) const -> size_t {
        return hash<qps::Synonym>{}(*syn);
    }
};

} // namespace std