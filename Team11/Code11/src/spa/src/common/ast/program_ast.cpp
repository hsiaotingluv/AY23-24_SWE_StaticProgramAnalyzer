#include "common/ast/program_ast.hpp"

namespace sp {
auto ProgramNode::get_children() -> std::vector<std::shared_ptr<AstNode>> {
    // As V[T] is invariant with V[K] given T <: K, C++ cannot automatically upcast them
    return procedures;
}

[[nodiscard]] auto ProgramNode::get_node_name() const -> std::string {
    return "ProgramNode";
}

[[nodiscard]] auto ProgramNode::identifier() const -> std::stringstream {
    auto ss = std::stringstream();
    ss << get_node_name() << "(";
    for (const auto& proc : procedures) {
        ss << *proc << ", ";
    }
    ss << ")";
    return ss;
}

[[nodiscard]] auto ProgramNode::to_xml() const -> std::string {
    auto xml = "<" + get_node_name() + ">";
    for (const auto& proc : procedures) {
        xml += proc->to_xml();
    }
    xml += "</" + get_node_name() + ">";
    return xml;
}
} // namespace sp