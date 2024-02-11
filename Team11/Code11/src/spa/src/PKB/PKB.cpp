#include "PKB/PKB.h"

PKB::PKB() {
    entity_store = std::make_shared<EntityStore>();
//    statement_store = std::make_shared<StatementStore>();
//    follows_store = std::make_shared<FollowsStore>();
//    parent_store = std::make_shared<ParentStore>();
    modifies_store = std::make_shared<ModifiesStore>();
    uses_store = std::make_shared<UsesStore>();
}
