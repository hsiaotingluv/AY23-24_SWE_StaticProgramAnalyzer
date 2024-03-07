#pragma once

#include "entity.h"
#include <string>
#include <utility>

class Procedure : public Entity {
  public:
    using Entity::Entity;
};

namespace std {
template <>
struct hash<Procedure> : Entity::HashFunction {};
}; // namespace std