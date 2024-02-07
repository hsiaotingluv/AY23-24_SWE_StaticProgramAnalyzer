#include "qps/parser/entities/synonym.hpp"
#include <algorithm>

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
                return T{x.name};
            }

            return std::nullopt;
        },
        synonym);
}

auto find_syn(const Synonyms& declared_synonyms, std::string syn_name) -> std::optional<Synonym> {
    const auto syn_it = std::find_if(declared_synonyms.begin(), declared_synonyms.end(), [&syn_name](auto&& x) -> bool {
        return std::visit(
            [&syn_name](const auto& decl) -> bool {
                return decl.name == syn_name;
            },
            x);
    });
    return syn_it == declared_synonyms.end() ? std::nullopt : std::make_optional(*syn_it);
}

auto find_stmt_syn(const Synonyms& declared_synonyms, std::string syn_name) -> std::optional<StmtSynonym> {
    const auto maybe_syn = find_syn(declared_synonyms, syn_name);
    if (!maybe_syn.has_value() || !is_stmt_synonym(maybe_syn.value())) {
        return std::nullopt;
    }
    return get_stmt_synonym(maybe_syn.value());
}

} // namespace qps
