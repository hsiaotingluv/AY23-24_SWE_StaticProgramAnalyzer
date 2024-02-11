#pragma once

#include "Entity.h"
#include <string>
#include <utility>

class Variable : public Entity {
  public:
    explicit Variable(std::string name) : Entity(std::move(name)) {}
};

namespace std {
    template<> struct hash<Variable> : Entity::HashFunction {};
};