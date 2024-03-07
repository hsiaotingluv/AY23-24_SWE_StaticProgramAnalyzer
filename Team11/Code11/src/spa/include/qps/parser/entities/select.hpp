#pragma once

#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {
using Elem = std::variant<std::shared_ptr<Synonym>>;

} // namespace qps