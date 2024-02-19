#pragma once

namespace sp {
class Expr {
  public:
    [[nodiscard]] virtual auto get_postfix_token() const -> std::string = 0;
    virtual ~Expr() = default;
};
} // namespace sp