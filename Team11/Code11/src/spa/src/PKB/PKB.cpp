#include "PKB/PKB.h"
#include "PKB/Facades/ReadFacade.h"
#include "PKB/Facades/WriteFacade.h"

PKB::PKB()
    : entity_store(std::make_shared<EntityStore>()), statement_store(std::make_shared<StatementStore>()),
      follows_store(std::make_shared<FollowsStore>()), parent_store(std::make_shared<ParentStore>()),
      modifies_store(std::make_shared<ModifiesStore>()), uses_store(std::make_shared<UsesStore>()),
      next_store(std::make_shared<NextStore>()), calls_store(std::make_shared<CallsStore>()) {
}

auto PKB::create_facades() -> std::tuple<std::shared_ptr<ReadFacade>, std::shared_ptr<WriteFacade>> {
    PKB* raw_ptr_pkb = new PKB();
    auto pkb = std::shared_ptr<PKB>(raw_ptr_pkb);
    auto readFacade = std::make_shared<ReadFacade>(pkb);
    auto writeFacade = std::make_shared<WriteFacade>(pkb);

    return {std::move(readFacade), std::move(writeFacade)};
};
