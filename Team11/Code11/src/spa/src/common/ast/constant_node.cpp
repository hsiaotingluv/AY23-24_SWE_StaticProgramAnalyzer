#include "common/ast/factor_ast.hpp"

namespace sp {
auto ConstantNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {};
}

[[nodiscard]] auto ConstantNode::get_node_name() const -> std::string {
    return "ConstantNode";
}

[[nodiscard]] auto ConstantNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << integer << ")";
    return ss;
}

[[nodiscard]] auto ConstantNode::to_xml() const -> std::string {
    return "<" + get_node_name() + " integer=\"" + integer + "\" />";
}

[[nodiscard]] auto ConstantNode::get_postfix() const -> std::string {
    return integer + " ";
}

auto ConstantNode::populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void {
    write_facade->add_constant(integer);
}
} // namespace sp