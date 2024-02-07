#pragma once

#include <ostream>
#include <string>

namespace qps {
struct WildCard {
    friend auto operator<<(std::ostream& os, const WildCard& wildCard) -> std::ostream& {
        os << "_";
        return os;
    }
};

struct IDENT {
    std::string value;

    friend auto operator<<(std::ostream& os, const IDENT& ident) -> std::ostream& {
        os << ident.value;
        return os;
    }

    friend auto operator==(const IDENT& lhs, const IDENT& rhs) -> bool {
        return lhs.value.compare(rhs.value) == 0;
    }

    friend auto operator==(const IDENT& lhs, const std::string& rhs) -> bool {
        return lhs.value.compare(rhs) == 0;
    }

    friend auto operator==(const std::string& lhs, const IDENT& rhs) -> bool {
        return lhs.compare(rhs.value) == 0;
    }
};

struct QuotedIdent {
    std::string value;

    friend auto operator<<(std::ostream& os, const QuotedIdent& quotedIdent) -> std::ostream& {
        os << "\"" << quotedIdent.value << "\"";
        return os;
    }

    friend auto operator==(const QuotedIdent& lhs, const QuotedIdent& rhs) -> bool {
        return lhs.value.compare(rhs.value) == 0;
    }

    friend auto operator==(const QuotedIdent& lhs, const std::string& rhs) -> bool {
        return lhs.value.compare(rhs) == 0;
    }

    friend auto operator==(const std::string& lhs, const QuotedIdent& rhs) -> bool {
        return lhs.compare(rhs.value) == 0;
    }
};

} // namespace qps