#pragma once

#include "sp/parser/ast/ast.hpp"

namespace sp {
// Generic Binary Expression with 2 operands
class BinopNode : public sp::AstNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit BinopNode(sp::NodeType T) : AstNode(T) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {left, right};
    }
};

class MulNode : public BinopNode {
  public:
    explicit MulNode() : BinopNode(NodeType::Mul) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "MulBinopNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class DivNode : public BinopNode {
  public:
    explicit DivNode() : BinopNode(NodeType::Div) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "DivBinopNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class ModNode : public BinopNode {
  public:
    explicit ModNode() : BinopNode(NodeType::Mod) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "ModBinopNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class AddNode : public BinopNode {
  public:
    explicit AddNode() : BinopNode(NodeType::Add) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "AddBinopNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class SubNode : public BinopNode {
  public:
    explicit SubNode() : BinopNode(NodeType::Sub) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "SubBinopNode(" << *left << ", " << *right << ")";
        return ss;
    }
};
} // namespace sp
