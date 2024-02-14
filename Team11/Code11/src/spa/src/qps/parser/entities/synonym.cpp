#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

namespace qps {
auto is_var_syn(const Synonym& syn) -> bool {
    return std::holds_alternative<VarSynonym>(syn);
}

auto is_var_syn(const EntRef& ent_ref) -> bool {
    return is_synonym(ent_ref) && is_var_syn(std::get<Synonym>(ent_ref));
}

auto is_proc_syn(const Synonym& syn) -> bool {
    return std::holds_alternative<ProcSynonym>(syn);
}

auto is_proc_syn(const EntRef& ent_ref) -> bool {
    return is_synonym(ent_ref) && is_proc_syn(std::get<Synonym>(ent_ref));
}

auto is_assign_syn(const Synonym& syn) -> bool {
    return std::holds_alternative<AssignSynonym>(syn);
}

auto is_stmt_synonym(Synonym synonym) -> bool {
    return std::visit(
        [](const auto& x) -> bool {
            using T = std::decay_t<decltype(x)>;
            return is_one_of<T, StmtSynonym>();
        },
        synonym);
}

auto get_stmt_synonym(Synonym synonym) -> std::optional<StmtSynonym> {
    return std::visit(
        [](auto&& x) -> std::optional<StmtSynonym> {
            using T = std::decay_t<decltype(x)>;
            if constexpr (is_one_of<T, StmtSynonym>()) {
                return T{x.get_name()};
            }

            return std::nullopt;
        },
        synonym);
}

} // namespace qps
