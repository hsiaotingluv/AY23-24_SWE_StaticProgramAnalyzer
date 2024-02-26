#include "common/ast/binary_node_ast.hpp"

namespace sp {
auto BinopNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {left, right};
}

[[nodiscard]] auto BinopNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << *left << ", " << *right << ")";
    return ss;
}

[[nodiscard]] auto BinopNode::to_xml() const -> std::string {
    auto start_xml = "<" + get_node_name() + ">";
    auto left_xml = left->to_xml();
    auto right_xml = right->to_xml();
    auto end_xml = "</" + get_node_name() + ">";

    return start_xml + left_xml + right_xml + end_xml;
}

[[nodiscard]] auto BinopNode::get_postfix() const -> std::string {
    auto left_postfix = left->get_postfix();
    auto right_postfix = right->get_postfix();
    return left_postfix + right_postfix + operator_token + " ";
}
} // namespace sp