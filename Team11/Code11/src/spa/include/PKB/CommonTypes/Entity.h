#pragma once

#include <unordered_set>
#include <string>
#include <utility>
#include <typeinfo>

class Entity {
  public:
    explicit Entity(std::string name) : name(std::move(name)) {};

    bool operator==(Entity const& other) const {
        return name == other.name;
    };

    [[nodiscard]] std::string getName() const { return name; }

    struct HashFunction {
        size_t operator()(Entity const& e) const {
            return std::hash<std::string>()(e.name);
        }
    };

  protected:
    std::string name;
};

namespace std {
    template<> struct hash<Entity> : Entity::HashFunction {};
};