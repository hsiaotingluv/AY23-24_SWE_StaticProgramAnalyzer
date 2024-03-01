#include "common/ast/statement_list_ast.hpp"

namespace sp {

auto StatementListNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    return statements;
}

[[nodiscard]] auto StatementListNode::get_node_name() const -> std::string {
    return "StatementListNode";
}

[[nodiscard]] auto StatementListNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(";
    for (const auto& stmt : statements) {
        ss << *stmt << ", ";
    }
    ss << ")";
    return ss;
}

[[nodiscard]] auto StatementListNode::to_xml() const -> std::string {
    auto xml = "<" + get_node_name() + ">";
    for (const auto& stmt : statements) {
        xml += stmt->to_xml();
    }
    xml += "</" + get_node_name() + ">";
    return xml;
}

auto StatementListNode::build_cfg(std::shared_ptr<Cfg> cfg) -> void {
    for (const auto& node : statements) {
        auto stmt_node = std::dynamic_pointer_cast<CfgMixin>(node);
        stmt_node->build_cfg(cfg);
    }
}
} // namespace sp