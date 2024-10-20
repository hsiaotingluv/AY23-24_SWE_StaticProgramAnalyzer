#pragma once

#include "qps/parser/entities/primitives.hpp"
#include <variant>

namespace qps::untyped {
struct UntypedSynonym {
    IDENT name;

  public:
    explicit UntypedSynonym(IDENT name) : name(std::move(name)) {
    }

    auto operator==(const UntypedSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }

    [[nodiscard]] auto get_name_string() const -> std::string {
        return name.get_value();
    }

    friend auto operator<<(std::ostream& os, const UntypedSynonym& synonym) -> std::ostream& {
        os << synonym.name;
        return os;
    }
};

using UntypedStmtRef = std::variant<UntypedSynonym, WildCard, Integer>;
using UntypedEntRef = std::variant<UntypedSynonym, WildCard, QuotedIdent>;
using UntypedStmtEntRef = std::variant<UntypedSynonym, WildCard, Integer, QuotedIdent>;
} // namespace qps::untyped
