#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <ostream>
#include <variant>

namespace qps {
auto reject_wildcard(const StmtRef& stmt_ref) -> std::optional<StmtRefNoWildcard> {
    return std::visit(
        [](auto&& arg) -> std::optional<StmtRefNoWildcard> {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, WildCard>) {
                return std::nullopt;
            } else {
                return std::make_optional(StmtRefNoWildcard{arg});
            }
        },
        stmt_ref);
}

auto to_var_ref(const EntRef& ent_ref) -> std::optional<VarRef> {
    return std::visit(overloaded{[](const std::shared_ptr<Synonym>& x) -> std::optional<VarRef> {
                                     auto try_ptr = std::dynamic_pointer_cast<VarSynonym>(x);
                                     if (!try_ptr) {
                                         return std::nullopt;
                                     }
                                     return VarRef{try_ptr};
                                 },
                                 [](const auto& x) -> std::optional<VarRef> {
                                     return x;
                                 }},
                      ent_ref);
}

auto operator<<(std::ostream& os, const Follows& follows) -> std::ostream& {
    os << "Follows(" << follows.stmt1 << ", " << follows.stmt2 << ")";
    return os;
}

auto operator<<(std::ostream& os, const FollowsT& followsT) -> std::ostream& {
    os << "Follows*(" << followsT.stmt1 << ", " << followsT.stmt2 << ")";
    return os;
}

auto operator<<(std::ostream& os, const Parent& parent) -> std::ostream& {
    os << "Parent(" << parent.stmt1 << ", " << parent.stmt2 << ")";
    return os;
}

auto operator<<(std::ostream& os, const ParentT& parentT) -> std::ostream& {
    os << "Parent*(" << parentT.stmt1 << ", " << parentT.stmt2 << ")";
    return os;
}

auto operator<<(std::ostream& os, const UsesS& uses) -> std::ostream& {
    os << "UsesS(" << uses.stmt << ", " << uses.ent << ")";
    return os;
}

#ifndef MILESTONE1
auto operator<<(std::ostream& os, const UsesP& uses) -> std::ostream& {
    os << "UsesP(" << uses.ent1 << ", " << uses.ent2 << ")";
    return os;
}
#endif

auto operator<<(std::ostream& os, const ModifiesS& modifies) -> std::ostream& {
    os << "ModifiesS(" << modifies.stmt << ", " << modifies.ent << ")";
    return os;
}

#ifndef MILESTONE1
auto operator<<(std::ostream& os, const ModifiesP& modifies) -> std::ostream& {
    os << "ModifiesP(" << modifies.ent1 << ", " << modifies.ent2 << ")";
    return os;
}
#endif

} // namespace qps
