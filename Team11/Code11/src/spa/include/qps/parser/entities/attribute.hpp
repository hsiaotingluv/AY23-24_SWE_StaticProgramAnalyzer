#pragma once

#include "qps/parser/entities/attribute_name.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"
#include <memory>

namespace qps {

struct AttrRef {
    enum class Type { Integer, Name };

    std::shared_ptr<Synonym> synonym;
    AttrName attr_name;
    Type type;

    AttrRef(std::shared_ptr<Synonym> synonym, AttrName attr_name, Type type)
        : synonym(std::move(synonym)), attr_name(attr_name), type(type) {
    }

    auto type_equals(const AttrRef& other) const -> bool {
        return type == other.type;
    }

    auto operator==(const AttrRef& other) const -> bool {
        return synonym == other.synonym && attr_name == other.attr_name && type == other.type;
    }

    friend auto operator<<(std::ostream& os, const AttrRef& attr_ref) -> std::ostream& {
        os << attr_ref.synonym << "." << attr_ref.attr_name;
        return os;
    }
};

using TypeRefList = TypeList<QuotedIdent, Integer, AttrRef>;
using TypedRef = type_list_to_variant_t<TypeRefList>;
} // namespace qps