#include "pkb/PKB.h"
#include "sp/TNode.h"

//auto PKB::setProcToAST(PROC p, TNode *r) -> int {
//    return 0;
//}
//
//auto PKB::getRootAST(PROC p) -> TNode * {
//    return nullptr;
//}

Pkb::Pkb() {
    follows_store_ = std::make_shared<FollowsStore>();
    parent_store_ = std::make_shared<ParentStore>();
}

Pkb::~Pkb() = default;