#include "pkb/pkb.h"
#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"

#include <tuple>
#include <vector>

PKB::PKB()
    : entity_store(std::make_shared<EntityStore>()), statement_store(std::make_shared<StatementStore>()),
      direct_follows_store(std::make_shared<DirectFollowsStore>()),
      follows_star_store(std::make_shared<FollowsStarStore>()),
      direct_parent_store(std::make_shared<DirectParentStore>()),
      parent_star_store(std::make_shared<ParentStarStore>()),
      procedure_modifies_store(std::make_shared<ProcedureModifiesStore>()),
      statement_modifies_store(std::make_shared<StatementModifiesStore>()),
      procedure_uses_store(std::make_shared<ProcedureUsesStore>()),
      statement_uses_store(std::make_shared<StatementUsesStore>()),
      assignment_store(std::make_shared<AssignmentStore>()), next_store(std::make_shared<NextStore>()),
      direct_calls_store(std::make_shared<DirectCallsStore>()), calls_star_store(std::make_shared<CallsStarStore>()),
      if_var_store(std::make_shared<IfVarStore>()), while_var_store(std::make_shared<WhileVarStore>()) {
}

auto PKB::create_facades() -> std::tuple<std::shared_ptr<ReadFacade>, std::shared_ptr<WriteFacade>> {
    auto pkb = std::make_shared<PKB>();
    auto read_facade = std::make_shared<ReadFacade>(pkb);
    auto write_facade = std::make_shared<WriteFacade>(pkb);

    return {std::move(read_facade), std::move(write_facade)};
};

auto insert(std::vector<std::tuple<StatementNumber, StatementNumber>>& rs, const StatementNumber& s1,
            const StatementNumber& s2) -> void {
    rs.emplace_back(s1, s2);
}

auto insert(std::vector<std::tuple<StatementNumber, StatementNumber>>& rs, const StatementNumber& s1,
            const std::unordered_set<StatementNumber>& s2) -> void {
    for (const auto& s : s2) {
        insert(rs, s1, s);
    }
}

auto insert_procedure(std::vector<std::tuple<Procedure, Procedure>>& rs, const Procedure& p1, const Procedure& p2)
    -> void {
    rs.emplace_back(p1, p2);
}

auto insert_procedure(std::vector<std::tuple<Procedure, Procedure>>& rs, const Procedure& p1,
                      const std::unordered_set<Procedure>& p2) -> void {
    for (const auto& p : p2) {
        insert_procedure(rs, p1, p);
    }
}

template <class DirectStore, class StarStore>
void PKB::populate_star_from_direct(DirectStore direct_store, StarStore star_store) {
    // extract all the direct relationships into a vector of tuples
    std::vector<std::tuple<StatementNumber, StatementNumber>> relationships;

    for (const auto& pair : direct_store->get_all()) {
        insert(relationships, pair.first, pair.second);
    }

    // sort relationships by the second element of the tuple
    std::sort(relationships.begin(), relationships.end(), [](const auto& a, const auto& b) {
        return stoi(std::get<1>(a)) < stoi(std::get<1>(b));
    });

    // populate star_store, starting from the last element of relationships
    for (auto it = relationships.rbegin(); it != relationships.rend(); ++it) {
        const auto& [s1, s2] = *it;
        star_store->add(s1, s2);

        // add all the star relationships from s2 to s1
        for (const auto& s3 : star_store->get_vals_by_key(s2)) {
            star_store->add(s1, s3);
        }
    }
}

template <class DirectStore, class StarStore>
void PKB::populate_call_star_from_direct(DirectStore direct_store, StarStore star_store) {
    // extract all the direct relationships into a vector of tuples
    std::vector<std::tuple<Procedure, Procedure>> relationships;

    for (const auto& pair : direct_store->get_all()) {
        insert_procedure(relationships, pair.first, pair.second);
    }

    // populate star_store, starting from the last element of relationships
    for (auto it = relationships.rbegin(); it != relationships.rend(); ++it) {
        const auto& [s1, s2] = *it;
        star_store->add(s1, s2);

        // add all the star relationships from s2 to s1
        for (const auto& s3 : star_store->get_vals_by_key(s2)) {
            star_store->add(s1, s3);
        }
    }
}

void PKB::finalise_pkb() {
    populate_star_from_direct(direct_follows_store, follows_star_store);
    populate_star_from_direct(direct_parent_store, parent_star_store);
    populate_call_star_from_direct(direct_calls_store, calls_star_store);
}
