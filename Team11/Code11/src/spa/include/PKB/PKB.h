#pragma once

#include "PKB/Stores/CallsStore.h"
#include "PKB/Stores/EntityStore.h"
#include "PKB/Stores/FollowsStore.h"
#include "PKB/Stores/ModifiesStore.h"
#include "PKB/Stores/NextStore.h"
#include "PKB/Stores/ParentStore.h"
#include "PKB/Stores/StatementStore.h"
#include "PKB/Stores/UsesStore.h"
#include <memory>
#include <tuple>

// Forward declaration of facades
class ReadFacade;
class WriteFacade;

class PKB {
  public:
    static std::tuple<std::shared_ptr<ReadFacade>, std::shared_ptr<WriteFacade>> create_facades();

  private:
    PKB();

    std::shared_ptr<EntityStore> entity_store;
    std::shared_ptr<StatementStore> statement_store;
    std::shared_ptr<FollowsStore> follows_store;
    std::shared_ptr<ParentStore> parent_store;
    std::shared_ptr<ModifiesStore> modifies_store;
    std::shared_ptr<UsesStore> uses_store;
    std::shared_ptr<NextStore> next_store;
    std::shared_ptr<CallsStore> calls_store;

    friend class ReadFacade;

    friend class WriteFacade;
};
