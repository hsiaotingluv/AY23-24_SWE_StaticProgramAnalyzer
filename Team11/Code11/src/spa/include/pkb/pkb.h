#pragma once

#include "pkb/stores/assignment_store.h"
#include "pkb/stores/calls_store.h"
#include "pkb/stores/entity_store.h"
#include "pkb/stores/follows_store/direct_follows_store.h"
#include "pkb/stores/follows_store/follows_star_store.h"
#include "pkb/stores/modifies_store/procedure_modifies_store.h"
#include "pkb/stores/modifies_store/statement_modifies_store.h"
#include "pkb/stores/next_store.h"
#include "pkb/stores/parent_store/direct_parent_store.h"
#include "pkb/stores/parent_store/parent_star_store.h"
#include "pkb/stores/statement_store.h"
#include "pkb/stores/uses_store/procedure_uses_store.h"
#include "pkb/stores/uses_store/statement_uses_store.h"
#include <memory>
#include <tuple>

// Forward declaration of facades
class ReadFacade;
class WriteFacade;

class PKB {
  public:
    static std::tuple<std::shared_ptr<ReadFacade>, std::shared_ptr<WriteFacade>> create_facades();

    std::unordered_set<std::string> filter_statements_by_type(const std::unordered_set<std::string>& stmts,
                                                              StatementType statementType) const;

    void finalise_pkb();

    PKB();

  private:
    std::shared_ptr<EntityStore> entity_store;
    std::shared_ptr<StatementStore> statement_store;
    std::shared_ptr<DirectFollowsStore> direct_follows_store;
    std::shared_ptr<FollowsStarStore> follows_star_store;
    std::shared_ptr<DirectParentStore> direct_parent_store;
    std::shared_ptr<ParentStarStore> parent_star_store;
    std::shared_ptr<ProcedureModifiesStore> procedure_modifies_store;
    std::shared_ptr<StatementModifiesStore> statement_modifies_store;
    std::shared_ptr<ProcedureUsesStore> procedure_uses_store;
    std::shared_ptr<StatementUsesStore> statement_uses_store;
    std::shared_ptr<NextStore> next_store;
    std::shared_ptr<CallsStore> calls_store;
    std::shared_ptr<AssignmentStore> assignment_store;

    template <class DirectStore, class StarStore>
    void populate_star_from_direct(DirectStore direct_store, StarStore star_store);

    friend class ReadFacade;

    friend class WriteFacade;
};
