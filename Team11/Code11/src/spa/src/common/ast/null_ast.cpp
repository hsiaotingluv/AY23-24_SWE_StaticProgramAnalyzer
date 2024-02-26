#include "common/ast/null_ast.hpp"

namespace sp {

auto NullNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {};
}

[[nodiscard]] auto NullNode::get_node_name() const -> std::string {
    return "NullNode";
}

[[nodiscard]] auto NullNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "()";
    return ss;
}

[[nodiscard]] auto NullNode::to_xml() const -> std::string {
    return "<" + get_node_name() + " />";
}
} // namespace sp