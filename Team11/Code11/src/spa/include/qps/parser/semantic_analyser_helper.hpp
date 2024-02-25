#pragma once

#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include <optional>

namespace qps::details {
// Forward declarations
auto enforce_unique_declarations(const Synonyms& declarations)
    -> std::optional<std::unordered_map<std::string, std::shared_ptr<Synonym>>>;

auto is_synonym_declared(const Synonyms& declarations,
                         const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                         const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<Synonym>>;

auto get_stmt_synonym(const Synonyms& declarations,
                      const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                      const untyped::UntypedSynonym& reference) -> std::optional<std::shared_ptr<StmtSynonym>>;
} // namespace qps::details
