#pragma once

#include <string>
#include <utility>

class Entity {
  public:
    explicit Entity(std::string name) : name(std::move(name)){};
    Entity() = default;

    bool operator==(Entity const& other) const {
        return name == other.name;
    };

    [[nodiscard]] std::string get_name() const {
        return name;
    }

    struct HashFunction {
        size_t operator()(Entity const& e) const {
            return std::hash<std::string>()(e.name);
        }
    };

  private:
    std::string name;
};

namespace std {
template <>
struct hash<Entity> : Entity::HashFunction {};
}; // namespace std