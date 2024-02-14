#pragma once

#include "entity.h"
#include <string>
#include <utility>

class Procedure : public Entity {
  public:
    explicit Procedure(std::string name) : Entity(std::move(name)) {
    }
};

namespace std {
template <>
struct hash<Procedure> : Entity::HashFunction {};
}; // namespace std