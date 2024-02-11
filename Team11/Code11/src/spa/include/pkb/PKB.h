#pragma once

#include "pkb/Stores/EntityStore.h"
#include "pkb/Stores/FollowsStore.h"
#include "pkb/Stores/ParentStore.h"
#include <memory>

class PKB {
  public:
    PKB();

    ~PKB();

  private:
    std::shared_ptr<EntityStore> entity_store;
    //    std::shared_ptr<StatementStore> statement_store;
    //    std::shared_ptr<FollowsStore> follows_store;
    //    std::shared_ptr<ParentStore> parent_store;
    //    std::shared_ptr<ModifiesStore> modifies_store;
    //    std::shared_ptr<UsesStore> uses_store;

    friend class ReadFacade;
    friend class WriteFacade;
};