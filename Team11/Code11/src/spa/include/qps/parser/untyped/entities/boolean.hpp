#pragma once

#include <ostream>

namespace qps::untyped {

struct UntypedBoolean {
    static constexpr auto keyword = "BOOLEAN";

    friend auto operator<<(std::ostream& os, const UntypedBoolean&) -> std::ostream& {
        os << "UntypedBOOLEAN()";
        return os;
    }
};

inline auto operator==(const UntypedBoolean&, const UntypedBoolean&) -> bool {
    return true;
}
} // namespace qps::untyped