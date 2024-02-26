#include "common/ast/logical_ast.hpp"

namespace sp {
auto LogicalNotNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {cond_expr};
}

[[nodiscard]] auto LogicalNotNode::get_node_name() const -> std::string {
    return "LogicalNotNode";
}

[[nodiscard]] auto LogicalNotNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << *cond_expr << ")";
    return ss;
}

[[nodiscard]] auto LogicalNotNode::to_xml() const -> std::string {
    auto start_xml = "<" + get_node_name() + ">";
    auto cond_expr_xml = cond_expr->to_xml();
    auto end_xml = "</" + get_node_name() + ">";

    return start_xml + cond_expr_xml + end_xml;
}
} // namespace sp