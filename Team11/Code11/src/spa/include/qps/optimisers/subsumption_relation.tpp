#pragma once

// Inject Subsumption relationship definition for the purpose of
// subsumption rewriting optimisation

#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/template_utils.hpp"
#include <type_traits>
#include <variant>

namespace qps {
template <typename T, typename U, std::enable_if_t<is_member_v<T, RelationshipList>, int> = 0,
          std::enable_if_t<is_member_v<U, RelationshipList>, int> = 0>
static auto type_subsume(const T&, const U&) -> bool {
    return false;
}

template <typename T, std::enable_if_t<is_member_v<T, RelationshipList>, int> = 0>
static auto type_subsume(const T&, const T&) -> bool {
    return true;
}

template <>
auto type_subsume(const Follows&, const FollowsT&) -> bool {
    return true;
}

template <>
auto type_subsume(const Parent&, const ParentT&) -> bool {
    return true;
}

template <>
auto type_subsume(const Calls&, const CallsT&) -> bool {
    return true;
}

template <>
auto type_subsume(const Next&, const NextT&) -> bool {
    return true;
}

static auto arg_subsume(const StmtRef& stmt_ref1, const StmtRef& stmt_ref2) -> bool {
    return (stmt_ref1 == stmt_ref2) || std::holds_alternative<WildCard>(stmt_ref2);
}

template <typename T, std::enable_if_t<is_member_v<T, TypeList<VarRef, ProcedureRef, EntRef>>, int> = 0>
static auto arg_subsume(const T& ent_ref1, const T& ent_ref2) -> bool {
    return (ent_ref1 == ent_ref2) || std::holds_alternative<WildCard>(ent_ref2);
}

template <typename T, typename U,
          std::enable_if_t<!is_member_v<T, DefaultStmtStmtList> || !is_member_v<U, DefaultStmtStmtList>, int> = 0>
static auto args_subsume(const T&, const U&) -> bool {
    return false;
}

template <typename T, typename U, std::enable_if_t<is_member_v<T, DefaultStmtStmtList>, int> = 0,
          std::enable_if_t<is_member_v<U, DefaultStmtStmtList>, int> = 0>
static auto args_subsume(const T& relationship1, const U& relationship2) -> bool {
    return arg_subsume(relationship1.stmt1, relationship2.stmt1) &&
           arg_subsume(relationship1.stmt2, relationship2.stmt2);
}

template <typename T, std::enable_if_t<is_member_v<T, DefaultProcProcList>, int> = 0>
static auto args_subsume(const T& relationship1, const T& relationship2) -> bool {
    return arg_subsume(relationship1.procedure1, relationship2.procedure1) &&
           arg_subsume(relationship1.procedure2, relationship2.procedure2);
}

template <typename T, std::enable_if_t<is_member_v<T, DefaultProcVarList>, int> = 0>
static auto args_subsume(const T& relationship1, const T& relationship2) -> bool {
    return relationship1.ent1 == relationship2.ent1 && arg_subsume(relationship1.ent2, relationship2.ent2);
}

template <typename T, std::enable_if_t<is_member_v<T, DefaultStmtEntList>, int> = 0>
static auto args_subsume(const T& relationship1, const T& relationship2) -> bool {
    return relationship1.stmt == relationship2.stmt && arg_subsume(relationship1.ent, relationship2.ent);
}
} // namespace qps