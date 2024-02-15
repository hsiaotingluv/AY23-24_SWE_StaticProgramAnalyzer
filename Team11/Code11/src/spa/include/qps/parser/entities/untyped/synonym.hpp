#pragma once

#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <variant>

namespace qps::untyped {
struct UntypedSynonym {
    IDENT name;

  public:
    explicit UntypedSynonym(IDENT name) : name(std::move(name)) {
    }

    explicit UntypedSynonym(std::string name) : name(std::move(name)) {
    }

    auto operator==(const UntypedSynonym& rhs) const -> bool {
        return name == rhs.name;
    }

    [[nodiscard]] auto get_name() const -> IDENT {
        return name;
    }

    auto operator<<(std::ostream& os) const -> std::ostream& {
        os << name;
        return os;
    }
};

using UntypedStmtRef = std::variant<UntypedSynonym, WildCard, Integer>;
using UntypedEntRef = std::variant<UntypedSynonym, WildCard, QuotedIdent>;
} // namespace qps::untyped
