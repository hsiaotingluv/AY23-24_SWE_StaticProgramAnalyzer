#include "qps/parser/analysers/semantic_analyser_helper.hpp"
#include "qps/parser/entities/attribute.hpp"
#include "qps/parser/entities/attribute_name.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>
#include <utility>

namespace qps ::detail {
auto enforce_unique_declarations(const Synonyms& declarations)
    -> std::optional<std::unordered_map<std::string, std::shared_ptr<Synonym>>> {
    std::unordered_map<std::string, std::shared_ptr<Synonym>> mapping;

    for (const auto& declaration : declarations) {
        mapping.insert({declaration->get_name_string(), declaration});
    }

    return mapping.size() == declarations.size() ? std::make_optional(mapping) : std::nullopt;
}

auto is_synonym_declared(const Synonyms&, const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                         const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<Synonym>> {

    if (mapping.find(reference.get_name_string()) == mapping.end()) {
        return std::nullopt;
    }
    return mapping.at(reference.get_name_string());
}

auto get_stmt_synonym(const Synonyms& declarations,
                      const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                      const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<StmtSynonym>> {
    const auto& maybe_syn = is_synonym_declared(declarations, mapping, reference);

    if (!maybe_syn.has_value()) {
        return std::nullopt;
    }

    const auto& maybe_stmt_syn = std::dynamic_pointer_cast<StmtSynonym>(maybe_syn.value());
    if (!maybe_stmt_syn) {
        return std::nullopt;
    }
    return maybe_stmt_syn;
}

auto validate_attr_ref(const Synonyms& declarations,
                       const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                       const untyped::UntypedAttrRef& attr_ref) -> std::optional<TypedRef> {
    const auto& maybe_synonym = is_synonym_declared(declarations, mapping, attr_ref.synonym);
    if (!maybe_synonym.has_value()) {
        return std::nullopt;
    }
    const auto& synonym = maybe_synonym.value();
    return std::visit(overloaded{[&synonym](const ProcName& attr_name) -> std::optional<TypedRef> {
                                     const auto proc_synonym = std::dynamic_pointer_cast<ProcSynonym>(synonym);
                                     const auto call_synonym = std::dynamic_pointer_cast<CallSynonym>(synonym);
                                     if (!proc_synonym && !call_synonym) {
                                         return std::nullopt;
                                     }
                                     return AttrRef{synonym, attr_name, AttrRef::Type::Name};
                                 },
                                 [&synonym](const VarName& attr_name) -> std::optional<TypedRef> {
                                     const auto var_synonym = std::dynamic_pointer_cast<VarSynonym>(synonym);
                                     const auto read_synonym = std::dynamic_pointer_cast<ReadSynonym>(synonym);
                                     const auto print_synonym = std::dynamic_pointer_cast<PrintSynonym>(synonym);
                                     if (!var_synonym && !read_synonym && !print_synonym) {
                                         return std::nullopt;
                                     }

                                     return AttrRef{synonym, attr_name, AttrRef::Type::Name};
                                 },
                                 [&synonym](const StmtNum& attr_name) -> std::optional<TypedRef> {
                                     if (!is_stmt_synonym(synonym)) {
                                         return std::nullopt;
                                     }
                                     return AttrRef{synonym, attr_name, AttrRef::Type::Integer};
                                 },
                                 [&synonym](const Value& attr_name) -> std::optional<TypedRef> {
                                     const auto const_synonym = std::dynamic_pointer_cast<ConstSynonym>(synonym);
                                     if (!const_synonym) {
                                         return std::nullopt;
                                     }
                                     return AttrRef{synonym, attr_name, AttrRef::Type::Integer};
                                 }},
                      attr_ref.attr_name);
}

auto validate_ref(const Synonyms& declarations,
                  const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                  const untyped::UntypedRef& ref) -> std::optional<TypedRef> {
    return std::visit(
        overloaded{[&declarations, &mapping](const untyped::UntypedAttrRef& attr_ref) -> std::optional<TypedRef> {
                       return validate_attr_ref(declarations, mapping, attr_ref);
                   },
                   [](const auto& x) -> std::optional<TypedRef> {
                       return x;
                   }},
        ref);
}

auto get_type(const QuotedIdent&) -> AttrRef::Type {
    return AttrRef::Type::Name;
}

auto get_type(const Integer&) -> AttrRef::Type {
    return AttrRef::Type::Integer;
}

auto get_type(const AttrRef& ref) -> AttrRef::Type {
    return ref.type;
}

auto validate_ref_combination(const TypedRef& ref1, const TypedRef& ref2)
    -> std::optional<std::pair<TypedRef, TypedRef>> {
    return std::visit(
        [](const auto& ref1, const auto& ref2) -> std::optional<std::pair<TypedRef, TypedRef>> {
            if (get_type(ref1) == get_type(ref2)) {
                return std::make_pair(ref1, ref2);
            }
            return std::nullopt;
        },
        ref1, ref2);
}
} // namespace qps::detail