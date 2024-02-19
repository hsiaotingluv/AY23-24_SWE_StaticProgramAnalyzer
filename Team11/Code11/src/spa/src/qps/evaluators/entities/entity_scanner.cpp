#include "qps/evaluators/entities/entity_scanner.hpp"

namespace qps {
auto scan_entities(std::shared_ptr<ReadFacade> read_facade, const std::shared_ptr<Synonym>& synonym)
    -> std::unordered_set<std::string> {
    return synonym->scan(read_facade);
}

} // namespace qps