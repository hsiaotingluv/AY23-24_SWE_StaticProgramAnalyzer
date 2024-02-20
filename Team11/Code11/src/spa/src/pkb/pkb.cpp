#include "pkb/pkb.h"
#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"

PKB::PKB()
    : entity_store(std::make_shared<EntityStore>()), statement_store(std::make_shared<StatementStore>()),
      follows_store(std::make_shared<FollowsStore>()), parent_store(std::make_shared<ParentStore>()),
      modifies_store(std::make_shared<ModifiesStore>()), uses_store(std::make_shared<UsesStore>()),
      next_store(std::make_shared<NextStore>()), calls_store(std::make_shared<CallsStore>()),
      assignment_store(std::make_shared<AssignmentStore>()) {
}

auto PKB::create_facades() -> std::tuple<std::shared_ptr<ReadFacade>, std::shared_ptr<WriteFacade>> {
    PKB* raw_ptr_pkb = new PKB();
    auto pkb = std::shared_ptr<PKB>(raw_ptr_pkb);
    auto read_facade = std::make_shared<ReadFacade>(pkb);
    auto write_facade = std::make_shared<WriteFacade>(pkb);

    return {std::move(read_facade), std::move(write_facade)};
};

std::unordered_set<std::string> PKB::filterStatementsByType(const std::unordered_set<std::string>& stmts,
                                                            StatementType statementType) const {
    std::unordered_set<std::string> filtered;
    for (const auto& stmt : stmts) {
        if (statement_store->get_statement_type(stmt) == statementType) {
            filtered.insert(stmt);
        }
    }
    return filtered;
}
