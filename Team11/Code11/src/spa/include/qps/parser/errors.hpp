#pragma once
#include <string>

namespace qps {

struct SyntaxError {
    std::string message;

    explicit SyntaxError(std::string message) : message(std::move(message)) {
    }
};

struct SemanticError {
    std::string message;

    explicit SemanticError(std::string message) : message(std::move(message)) {
    }
};
} // namespace qps
