#pragma once

#include "qps/parser/analysers/relationship_analyser.tpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"

namespace qps {
template <typename StmtStmtList, typename StmtEntList, typename EntEntList>
class RelationshipAnalyser {
  public:
    static auto analyse(const Synonyms& declarations,
                        const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                        const untyped::UntypedRelationship& relationship) -> std::optional<Relationship> {
        return std::visit(
            detail::untyped_relationship_visitor<StmtStmtList, StmtEntList, EntEntList>(declarations, mapping),
            relationship);
    }
};

} // namespace qps