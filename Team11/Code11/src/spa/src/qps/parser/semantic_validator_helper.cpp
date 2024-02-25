#include "qps/parser/semantic_analyser_helper.hpp"

namespace qps ::details {
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

} // namespace qps::details