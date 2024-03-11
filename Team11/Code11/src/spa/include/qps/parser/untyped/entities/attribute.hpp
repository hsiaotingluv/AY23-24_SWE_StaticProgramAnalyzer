#pragma once

#include "qps/parser/entities/attribute_name.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include <variant>

namespace qps::untyped {

struct UntypedAttrRef {
    UntypedSynonym synonym;
    AttrName attr_name;

    friend auto operator<<(std::ostream& os, const UntypedAttrRef& attr_ref) -> std::ostream& {
        os << attr_ref.synonym << "." << attr_ref.attr_name;
        return os;
    }
};

using UntypedRef = std::variant<QuotedIdent, Integer, UntypedAttrRef>;

inline auto operator==(const UntypedAttrRef& lhs, const UntypedAttrRef& rhs) -> bool {
    return lhs.synonym == rhs.synonym && lhs.attr_name == rhs.attr_name;
}
} // namespace qps::untyped