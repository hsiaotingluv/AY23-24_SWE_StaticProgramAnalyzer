#pragma once

#include "pkb/facades/read_facade.h"

#include "qps/parser/entities/primitives.hpp"
#include "qps/template_utils.hpp" // NOLINT

#include <cstddef>
#include <memory>
#include <ostream>
#include <type_traits>
#include <vector>

namespace qps {

class Synonym : public Ref {
    IDENT name;

    [[nodiscard]] virtual auto representation() const -> std::string = 0;

  public:
    virtual ~Synonym() = default;

    explicit Synonym(IDENT name) : name(std::move(name)) {
    }

    explicit Synonym(std::string name) : name(std::move(name)) {
    }

    [[nodiscard]] virtual auto get_keyword() const -> std::string = 0;

    [[nodiscard]] auto get_name() const -> IDENT;

    [[nodiscard]] auto get_name_string() const -> std::string;

    [[nodiscard]] virtual auto scan(const std::shared_ptr<ReadFacade>& read_facade) const
        -> std::unordered_set<std::string> = 0;

    auto operator==(const Synonym& rhs) const noexcept -> bool;

    friend auto operator<<(std::ostream& os, const Synonym& synonym) -> std::ostream&;

    auto operator<(const Synonym& rhs) const noexcept -> bool;
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

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class ReadSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "read";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class PrintSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "print";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class CallSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "call";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class WhileSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "while";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class IfSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "if";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class AssignSynonym final : public StmtSynonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "assign";

    using StmtSynonym::StmtSynonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class VarSynonym final : public Synonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "variable";

    using Synonym::Synonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class ConstSynonym final : public Synonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "constant";

    using Synonym::Synonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

class ProcSynonym final : public Synonym {
    [[nodiscard]] auto representation() const -> std::string override;

  public:
    static constexpr auto keyword = "procedure";

    using Synonym::Synonym;

    [[nodiscard]] auto get_keyword() const -> std::string override;

    [[nodiscard]] auto scan(const std::shared_ptr<ReadFacade>&) const -> std::unordered_set<std::string> override;
};

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
    return os << *ptr;
}

template auto operator==<Synonym>(const std::shared_ptr<Synonym>& lhs, const std::shared_ptr<Synonym>& rhs) -> bool;

template auto operator< <Synonym>(const std::shared_ptr<Synonym>& lhs, const std::shared_ptr<Synonym>& rhs) -> bool;

} // namespace qps

namespace std {

namespace {
// From boost/container_hash/hash.hpp
// See Mike Seymour in magic-numbers-in-boosthash-combine:
//     http://stackoverflow.com/questions/4948780
template <class T>
inline void hash_combine(std::size_t& seed, T const& v) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
} // namespace

template <>
struct hash<qps::Synonym> {
    auto operator()(const qps::Synonym& syn) const -> size_t {
        size_t hash_value = 0;
        hash_combine(hash_value, syn.get_keyword());
        hash_combine(hash_value, syn.get_name());
        return hash_value;
    }
};

template <>
struct hash<std::shared_ptr<qps::Synonym>> {
    auto operator()(const std::shared_ptr<qps::Synonym>& syn) const -> size_t {
        return hash<qps::Synonym>{}(*syn);
    }
};

} // namespace std