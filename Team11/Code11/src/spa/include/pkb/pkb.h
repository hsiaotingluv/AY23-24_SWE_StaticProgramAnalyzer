#pragma once

#include "pkb/stores/calls_store.h"
#include "pkb/stores/entity_store.h"
#include "pkb/stores/follows_store.h"
#include "pkb/stores/modifies_store.h"
#include "pkb/stores/next_store.h"
#include "pkb/stores/parent_store.h"
#include "pkb/stores/statement_store.h"
#include "pkb/stores/uses_store.h"
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
