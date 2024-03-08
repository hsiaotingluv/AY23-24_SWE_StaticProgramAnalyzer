#pragma once

#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <optional>
#include <ostream>
#include <utility>
#include <variant>

namespace qps {

using StmtRef = std::variant<WildCard, std::shared_ptr<StmtSynonym>, Integer>;
using EntRef = std::variant<WildCard, std::shared_ptr<Synonym>, QuotedIdent>;
using StmtRefNoWildcard = std::variant<std::shared_ptr<StmtSynonym>, Integer>;
using EntRefNoWildcard = std::variant<std::shared_ptr<Synonym>, QuotedIdent>;
using ProcedureRef = std::variant<WildCard, std::shared_ptr<ProcSynonym>, QuotedIdent>;
using ProcedureRefNoWildcard = std::variant<std::shared_ptr<ProcSynonym>, QuotedIdent>;
using VarRef = std::variant<WildCard, std::shared_ptr<VarSynonym>, QuotedIdent>;

auto reject_wildcard(const StmtRef& stmt_ref) -> std::optional<StmtRefNoWildcard>;
auto reject_wildcard(const EntRef& ent_ref) -> std::optional<EntRefNoWildcard>;
auto to_var_ref(const EntRef& ent_ref) -> std::optional<VarRef>;
auto to_proc_ref(const EntRefNoWildcard& ent_ref) -> std::optional<ProcedureRefNoWildcard>;
auto to_proc_ref(const EntRef& ent_ref) -> std::optional<ProcedureRef>;

struct Follows {
    /**
     * @brief A Follows relationship is a relationship between two statements where the first statement is executed
     * before the second statement.
     *
     * @param stmt1
     * @param stmt2
     */
    StmtRef stmt1;
    StmtRef stmt2;

    static constexpr auto keyword = "Follows";

  public:
    Follows(StmtRef stmt1, StmtRef stmt2) : stmt1(std::move(stmt1)), stmt2(std::move(stmt2)) {
    }

    friend auto operator<<(std::ostream& os, const Follows& follows) -> std::ostream&;

    auto operator==(const Follows& other) const -> bool {
        return stmt1 == other.stmt1 && stmt2 == other.stmt2;
    }
};

struct FollowsT {
    /**
     * @brief A Follows* relationship is a relationship between two statements where the first statement is executed
     * before the second statement, and there are no other statements that are executed between the two statements.
     *
     * @param stmt1
     * @param stmt2
     */
    StmtRef stmt1;
    StmtRef stmt2;

    static constexpr auto keyword = "Follows*";

  public:
    FollowsT(StmtRef stmt1, StmtRef stmt2) : stmt1(std::move(stmt1)), stmt2(std::move(stmt2)) {
    }

    friend auto operator<<(std::ostream& os, const FollowsT& followsT) -> std::ostream&;

    auto operator==(const FollowsT& other) const -> bool {
        return stmt1 == other.stmt1 && stmt2 == other.stmt2;
    }
};

struct Parent {
    /**
     * @brief A Parent relationship is a relationship between two statements where the first statement is the parent
     * statement of the second statement.
     *
     * @param stmt1
     * @param stmt2
     */
    StmtRef stmt1;
    StmtRef stmt2;

    static constexpr auto keyword = "Parent";

  public:
    Parent(StmtRef stmt1, StmtRef stmt2) : stmt1(std::move(stmt1)), stmt2(std::move(stmt2)) {
    }

    friend auto operator<<(std::ostream& os, const Parent& parent) -> std::ostream&;

    auto operator==(const Parent& other) const -> bool {
        return stmt1 == other.stmt1 && stmt2 == other.stmt2;
    }
};

struct ParentT {
    /**
     * @brief A Parent* relationship is a relationship between two statements where the first statement is the parent
     * statement of the second statement, and there are no other statements that are executed between the two
     * statements.
     *
     * @param stmt1
     * @param stmt2
     */

    StmtRef stmt1;
    StmtRef stmt2;

    static constexpr auto keyword = "Parent*";

    ParentT(StmtRef stmt1, StmtRef stmt2) : stmt1(std::move(stmt1)), stmt2(std::move(stmt2)) {
    }

    friend auto operator<<(std::ostream& os, const ParentT& parentT) -> std::ostream&;

    auto operator==(const ParentT& other) const -> bool {
        return stmt1 == other.stmt1 && stmt2 == other.stmt2;
    }
};

struct Calls {
    ProcedureRef procedure1;
    ProcedureRef procedure2;

    static constexpr auto keyword = "Calls";

    Calls(ProcedureRef procedure1, ProcedureRef procedure2)
        : procedure1(std::move(procedure1)), procedure2(std::move(procedure2)) {
    }

    friend auto operator<<(std::ostream& os, const Calls& call) -> std::ostream&;

    auto operator==(const Calls& other) const -> bool {
        return procedure1 == other.procedure1 && procedure2 == other.procedure2;
    }

    static auto construct(const EntRef& proc_ref1, const EntRef& proc_ref2) -> std::optional<Calls> {
        const auto maybe_proc1 = to_proc_ref(proc_ref1);
        if (!maybe_proc1.has_value()) {
            return std::nullopt;
        }

        const auto maybe_proc2 = to_proc_ref(proc_ref2);
        if (!maybe_proc2.has_value()) {
            return std::nullopt;
        }

        return Calls{maybe_proc1.value(), maybe_proc2.value()};
    }
};

struct CallsT {

    ProcedureRef procedure1;
    ProcedureRef procedure2;

    static constexpr auto keyword = "Calls*";

    CallsT(ProcedureRef procedure1, ProcedureRef procedure2)
        : procedure1(std::move(procedure1)), procedure2(std::move(procedure2)) {
    }

    friend auto operator<<(std::ostream& os, const CallsT& callT) -> std::ostream&;

    auto operator==(const CallsT& other) const -> bool {
        return procedure1 == other.procedure1 && procedure2 == other.procedure2;
    }

    static auto construct(const EntRef& proc_ref1, const EntRef& proc_ref2) -> std::optional<CallsT> {
        const auto maybe_proc1 = to_proc_ref(proc_ref1);
        if (!maybe_proc1.has_value()) {
            return std::nullopt;
        }

        const auto maybe_proc2 = to_proc_ref(proc_ref2);
        if (!maybe_proc2.has_value()) {
            return std::nullopt;
        }

        return CallsT{maybe_proc1.value(), maybe_proc2.value()};
    }
};

struct UsesS {

    /**
     * @brief A Uses relationship is a relationship between a statement and a variable where the statement uses the
     * variable.
     *
     * @param stmt
     * @param var
     */

    StmtRefNoWildcard stmt;
    VarRef ent;

    static constexpr auto keyword = "Uses";

    static auto construct(const StmtRef& stmt_ref, const EntRef& ent_ref) -> std::optional<UsesS> {
        const auto maybe_stmt = reject_wildcard(stmt_ref);
        if (!maybe_stmt.has_value()) {
            return std::nullopt;
        }

        const auto maybe_var = to_var_ref(ent_ref);
        if (!maybe_var.has_value()) {
            return std::nullopt;
        }

        return UsesS{maybe_stmt.value(), maybe_var.value()};
    }

    UsesS(StmtRefNoWildcard stmt, VarRef ent) : stmt(std::move(stmt)), ent(std::move(ent)) {
    }

    friend auto operator<<(std::ostream& os, const UsesS& usesS) -> std::ostream&;

    auto operator==(const UsesS& other) const -> bool {
        return stmt == other.stmt && ent == other.ent;
    }
};

struct UsesP {
    /**
     * @brief A Uses relationship is a relationship between a procedure and a variable where the procedure uses the
     * variable.
     *
     * @param proc
     * @param var
     */

    ProcedureRefNoWildcard ent1;
    VarRef ent2;

    static constexpr auto keyword = UsesS::keyword;

    UsesP(ProcedureRefNoWildcard ent1, VarRef ent2) : ent1(std::move(ent1)), ent2(std::move(ent2)) {
    }

    static auto construct(const EntRef& proc_ref, const EntRef& ent_ref) -> std::optional<UsesP> {
        const auto maybe_ent_ref = reject_wildcard(proc_ref);
        if (!maybe_ent_ref.has_value()) {
            return std::nullopt;
        }

        const auto maybe_proc = to_proc_ref(maybe_ent_ref.value());
        if (!maybe_proc.has_value()) {
            return std::nullopt;
        }

        const auto maybe_var = to_var_ref(ent_ref);
        if (!maybe_var.has_value()) {
            return std::nullopt;
        }

        return UsesP{maybe_proc.value(), maybe_var.value()};
    }

    friend auto operator<<(std::ostream& os, const UsesP& usesP) -> std::ostream&;

    auto operator==(const UsesP& other) const -> bool {
        return ent1 == other.ent1 && ent2 == other.ent2;
    }
};

struct ModifiesS {
    /**
     * @brief A Modifies relationship is a relationship between a statement and a variable where the statement modifies
     * the variable.
     *
     * @param stmt
     * @param var
     */

    StmtRefNoWildcard stmt;
    VarRef ent;

    static constexpr auto keyword = "Modifies";

    static auto construct(const StmtRef& stmt_ref, const EntRef& ent_ref) -> std::optional<ModifiesS> {
        const auto maybe_stmt = reject_wildcard(stmt_ref);
        if (!maybe_stmt.has_value()) {
            return std::nullopt;
        }

        const auto maybe_var = to_var_ref(ent_ref);
        if (!maybe_var.has_value()) {
            return std::nullopt;
        }

        return ModifiesS{maybe_stmt.value(), maybe_var.value()};
    }

    ModifiesS(StmtRefNoWildcard stmt, VarRef ent) : stmt(std::move(stmt)), ent(std::move(ent)) {
    }

    friend auto operator<<(std::ostream& os, const ModifiesS& modifiesS) -> std::ostream&;

    auto operator==(const ModifiesS& other) const -> bool {
        return stmt == other.stmt && ent == other.ent;
    }
};

struct ModifiesP {

    /**
     * @brief A Modifies relationship is a relationship between a procedure and a variable where the procedure modifies
     * the variable.
     *
     * @param proc
     * @param var
     */

    ProcedureRefNoWildcard ent1;
    VarRef ent2;

    static constexpr auto keyword = ModifiesS::keyword;

    ModifiesP(ProcedureRefNoWildcard ent1, VarRef ent2) : ent1(std::move(ent1)), ent2(std::move(ent2)) {
    }

    static auto construct(const EntRef& proc_ref, const EntRef& ent_ref) -> std::optional<ModifiesP> {
        const auto maybe_ent_ref = reject_wildcard(proc_ref);
        if (!maybe_ent_ref.has_value()) {
            return std::nullopt;
        }

        const auto maybe_proc = to_proc_ref(maybe_ent_ref.value());
        if (!maybe_proc.has_value()) {
            return std::nullopt;
        }

        const auto maybe_var = to_var_ref(ent_ref);
        if (!maybe_var.has_value()) {
            return std::nullopt;
        }

        return ModifiesP{maybe_proc.value(), maybe_var.value()};
    }

    friend auto operator<<(std::ostream& os, const ModifiesP& modifies) -> std::ostream&;

    auto operator==(const ModifiesP& other) const -> bool {
        return ent1 == other.ent1 && ent2 == other.ent2;
    }
};

using DefaultStmtStmtList = TypeList<FollowsT, Follows, ParentT, Parent>;
using DefaultStmtEntList = TypeList<UsesS, ModifiesS>;
using DefaultEntEntList = TypeList<UsesP, ModifiesP, Calls, CallsT>;

using RelationshipList = concat_t<concat_t<DefaultStmtStmtList, DefaultStmtEntList>, DefaultEntEntList>;
using Relationship = type_list_to_variant_t<RelationshipList>;
} // namespace qps
