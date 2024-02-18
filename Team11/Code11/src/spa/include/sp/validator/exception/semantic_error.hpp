#pragma once

#include <string>
#include <utility>

class SemanticError final : public std::exception {
    std::string message;

  public:
    explicit SemanticError(std::string message) : message(std::move(message)) {
    }

    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    }
};
