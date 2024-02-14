#pragma once

#include "entity.h"
#include <string>
#include <utility>

class Constant : public Entity {
  public:
    explicit Constant(std::string name) : Entity(std::move(name)) {
    }
};

namespace std {
template <>
struct hash<Constant> : Entity::HashFunction {};
}; // namespace std