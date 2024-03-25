#pragma once

#include "entity.h"

class Procedure : public Entity {
  public:
    using Entity::Entity;
};

namespace std {
template <>
struct hash<Procedure> : Entity::HashFunction {};
}; // namespace std