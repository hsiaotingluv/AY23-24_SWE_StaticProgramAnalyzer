#pragma once

#include "Entity.h"
#include <string>
#include <utility>

class Constant : public Entity {
  public:
    explicit Constant(std::string name) : Entity(std::move(name)) {}
};

namespace std {
    template<> struct hash<Constant> : Entity::HashFunction {};
};