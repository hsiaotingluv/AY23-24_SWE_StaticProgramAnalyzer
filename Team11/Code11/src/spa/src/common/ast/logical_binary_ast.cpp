#include "common/ast/logical_ast.hpp"

namespace sp {
auto LogicalBinaryNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {left, right};
}

[[nodiscard]] auto LogicalBinaryNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << *left << ", " << *right << ")";
    return ss;
}

[[nodiscard]] auto LogicalBinaryNode::to_xml() const -> std::string {
    auto start_xml = "<" + get_node_name() + ">";
    auto left_xml = left->to_xml();
    auto right_xml = right->to_xml();
    auto end_xml = "</" + get_node_name() + ">";

    return start_xml + left_xml + right_xml + end_xml;
}
} // namespace sp