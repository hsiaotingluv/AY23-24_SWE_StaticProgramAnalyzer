#include "common/ast/factor_ast.hpp"

namespace sp {
auto VarNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return {};
}

[[nodiscard]] auto VarNode::get_node_name() const -> std::string {
    return "VarNode";
}

[[nodiscard]] auto VarNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(" << name << ")";
    return ss;
}

[[nodiscard]] auto VarNode::to_xml() const -> std::string {
    return "<" + get_node_name() + " name=\"" + name + "\" />";
}

[[nodiscard]] auto VarNode::get_postfix() const -> std::string {
    return name + " ";
}

auto VarNode::populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void {
    write_facade->add_variable(name);
}
} // namespace sp