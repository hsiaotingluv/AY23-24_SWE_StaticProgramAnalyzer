#pragma once

#include <string>
#include <utility>

class ParsingError : public std::exception {
    std::string message;

  public:
    explicit ParsingError(std::string message) : message(std::move(message)) {
    }

    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    }
};
