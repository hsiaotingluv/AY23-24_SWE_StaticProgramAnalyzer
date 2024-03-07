#pragma once

#include "entity.h"
#include <string>
#include <utility>

class Constant : public Entity {
  public:
    using Entity::Entity;
};

namespace std {
template <>
struct hash<Constant> : Entity::HashFunction {};
}; // namespace std