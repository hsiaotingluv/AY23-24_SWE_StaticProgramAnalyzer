#pragma once

#include <cstdint>
#include <ostream>
#include <string>

namespace qps {

class Ref {};

class WildCard : public Ref {
  public:
    friend auto operator<<(std::ostream& os, const WildCard& wildCard) -> std::ostream& {
        os << "_";
        return os;
    }

    friend auto operator==(const WildCard& lhs, const WildCard& rhs) -> bool {
        return true;
    }
};

class IDENT {
  private:
    std::string value;

  public:
    explicit IDENT(std::string value) : value(std::move(value)) {
    }

    [[nodiscard]] auto get_value() const -> std::string {
        return value;
    }

    friend auto operator<<(std::ostream& os, const IDENT& ident) -> std::ostream& {
        os << ident.value;
        return os;
    }

    friend auto operator==(const IDENT& lhs, const IDENT& rhs) -> bool {
        return lhs.value == rhs.value;
    }

    friend auto operator==(const IDENT& lhs, const std::string& rhs) -> bool {
        return lhs.value == rhs;
    }

    friend auto operator==(const std::string& lhs, const IDENT& rhs) -> bool {
        return lhs == rhs.value;
    }
};

class QuotedIdent : public Ref {
    std::string value;

  public:
    explicit QuotedIdent(std::string value) : value(std::move(value)) {
    }

    [[nodiscard]] auto get_value() const -> std::string {
        return value;
    }

    friend auto operator<<(std::ostream& os, const QuotedIdent& quotedIdent) -> std::ostream& {
        os << "\"" << quotedIdent.value << "\"";
        return os;
    }

    friend auto operator==(const QuotedIdent& lhs, const QuotedIdent& rhs) -> bool {
        return lhs.value == rhs.value;
    }

    friend auto operator==(const QuotedIdent& lhs, const std::string& rhs) -> bool {
        return lhs.value == rhs;
    }

    friend auto operator==(const std::string& lhs, const QuotedIdent& rhs) -> bool {
        return lhs == rhs.value;
    }
};

struct Integer {
    uint32_t value;

    friend auto operator<<(std::ostream& os, const Integer& integer) -> std::ostream& {
        os << integer.value;
        return os;
    }

    friend auto operator==(const Integer& lhs, const Integer& rhs) -> bool {
        return lhs.value == rhs.value;
    }

    friend auto operator==(const Integer& lhs, const int& rhs) -> bool {
        return lhs.value == rhs;
    }

    friend auto operator==(const int& lhs, const Integer& rhs) -> bool {
        return lhs == rhs.value;
    }
};

} // namespace qps