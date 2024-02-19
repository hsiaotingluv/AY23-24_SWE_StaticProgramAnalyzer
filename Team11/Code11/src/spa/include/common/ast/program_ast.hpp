#pragma once

#include "common/ast/ast.hpp"

namespace sp {
class ProgramNode : public AstNode {
  public:
    std::vector<std::shared_ptr<AstNode>> procedures;

    explicit ProgramNode(std::vector<std::shared_ptr<AstNode>>& procedures)
        : AstNode(sp::NodeType::Program), procedures(std::move(procedures)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        // As V[T] is invariant with V[K] given T <: K, C++ cannot automatically upcast them
        return procedures;
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "ProgramNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(";
        for (const auto& proc : procedures) {
            ss << *proc << ", ";
        }
        ss << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        auto xml = "<" + get_node_name() + ">";
        for (auto const& proc : procedures) {
            xml += proc->to_xml();
        }
        xml += "</" + get_node_name() + ">";
        return xml;
    }
};
} // namespace sp