#pragma once

#include "entity.h"

class Variable : public Entity {
  public:
    using Entity::Entity;
};

namespace std {
template <>
struct hash<Variable> : Entity::HashFunction {};
} // namespace std