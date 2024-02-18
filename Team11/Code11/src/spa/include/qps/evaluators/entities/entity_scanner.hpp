#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/parser/entities/synonym.hpp"

#include <memory>
#include <unordered_set>

namespace qps {
auto scan_entities(std::shared_ptr<ReadFacade> read_facade, const std::shared_ptr<Synonym>& synonym)
    -> std::unordered_set<std::string>;

} // namespace qps