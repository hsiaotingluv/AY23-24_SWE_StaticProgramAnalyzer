#pragma once

#include "qps/parser/entities/synonym.hpp"

#include <ostream>
#include <utility>
#include <variant>

namespace qps {
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

    static constexpr auto keyword = "Follows";

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

    static constexpr auto keyword = "Parent";

    ParentT(StmtRef stmt1, StmtRef stmt2) : stmt1(std::move(stmt1)), stmt2(std::move(stmt2)) {
    }

    friend auto operator<<(std::ostream& os, const ParentT& parentT) -> std::ostream&;

    auto operator==(const ParentT& other) const -> bool {
        return stmt1 == other.stmt1 && stmt2 == other.stmt2;
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

    StmtRef stmt;
    EntRef ent;

    static constexpr auto keyword = "Uses";

    UsesS(StmtRef stmt, EntRef ent) : stmt(std::move(stmt)), ent(std::move(ent)) {
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

    EntRef ent1;
    EntRef ent2;

    static constexpr auto keyword = "Uses";

    UsesP(EntRef ent1, EntRef ent2) : ent1(std::move(ent1)), ent2(std::move(ent2)) {
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

    StmtRef stmt;
    EntRef ent;

    static constexpr auto keyword = "Modifies";

    ModifiesS(StmtRef stmt, EntRef ent) : stmt(std::move(stmt)), ent(std::move(ent)) {
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

    EntRef ent1;
    EntRef ent2;

    static constexpr auto keyword = "Modifies";

  public:
    ModifiesP(EntRef ent1, EntRef ent2) : ent1(std::move(ent1)), ent2(std::move(ent2)) {
    }

    friend auto operator<<(std::ostream& os, const ModifiesP& modifies) -> std::ostream&;

    auto operator==(const ModifiesP& other) const -> bool {
        return ent1 == other.ent1 && ent2 == other.ent2;
    }
};

using Relationship = std::variant<Follows, FollowsT, Parent, ParentT, UsesS, UsesP, ModifiesS, ModifiesP>;

inline auto operator<<(std::ostream& os, const Relationship& relationship) -> std::ostream& {
    std::visit(
        [&os](auto&& relationship) {
            os << relationship;
        },
        relationship);
    return os;
}
} // namespace qps
