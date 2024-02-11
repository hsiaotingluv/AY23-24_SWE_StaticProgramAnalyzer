#pragma once

#include "Entity.h"
#include <string>
#include <utility>

class Procedure : public Entity {
  public:
    explicit Procedure(std::string name) : Entity(std::move(name)) {}
};

namespace std {
    template<> struct hash<Procedure> : Entity::HashFunction {};
};