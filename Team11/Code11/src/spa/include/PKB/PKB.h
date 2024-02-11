#pragma once

#include "PKB/Stores/EntityStore.h"
#include "PKB/Stores/FollowsStore.h"
#include "PKB/Stores/ParentStore.h"
#include "PKB/Stores/ModifiesStore.h"
#include "PKB/Stores/UsesStore.h"
#include <memory>

class PKB {
public:
    PKB();

private:
    std::shared_ptr<EntityStore> entity_store;
//    std::shared_ptr<StatementStore> statement_store;
    std::shared_ptr<FollowsStore> follows_store;
    std::shared_ptr<ParentStore> parent_store;
    std::shared_ptr<ModifiesStore> modifies_store;
    std::shared_ptr<UsesStore> uses_store;

    friend class ReadFacade;

    friend class WriteFacade;
};