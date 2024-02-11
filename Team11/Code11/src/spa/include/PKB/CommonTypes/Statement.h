#pragma once

#include <unordered_set>
#include <string>
#include <utility>

class Statement {
  public:
    explicit Statement(std::string stmt_no) : stmt_no(std::move(stmt_no)) {};

    bool operator==(Statement const& other) const {
        return stmt_no == other.stmt_no;
    };

    struct HashFunction {
        size_t operator()(Statement const& s) const {
            return std::hash<std::string>()(s.stmt_no);
        }
    };

  private:
    std::string stmt_no;
};

namespace std {
    template<> struct hash<Statement> : Statement::HashFunction {};
};