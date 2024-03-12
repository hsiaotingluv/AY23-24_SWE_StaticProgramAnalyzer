#pragma once

#include "qps/parser/entities/attribute.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/untyped/entities/attribute.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include <optional>

namespace qps::detail {
// Forward declarations
auto enforce_unique_declarations(const Synonyms& declarations)
    -> std::optional<std::unordered_map<std::string, std::shared_ptr<Synonym>>>;

auto is_synonym_declared(const Synonyms& declarations,
                         const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                         const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<Synonym>>;

auto get_stmt_synonym(const Synonyms& declarations,
                      const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                      const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<StmtSynonym>>;

auto validate_ref(const Synonyms& declarations,
                  const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                  const untyped::UntypedRef& ref) -> std::optional<TypedRef>;

auto validate_ref_combination(const TypedRef&, const TypedRef&) -> std::optional<std::pair<TypedRef, TypedRef>>;

} // namespace qps::detail
