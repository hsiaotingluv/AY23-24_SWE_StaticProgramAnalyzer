#pragma once

#include "sp/parser/ast/ast.hpp"

// An Annotator is a class that is used to annotate the AST with additional information
// Annotator: AST -> AST
namespace sp {
class Annotator {
  public:
    virtual auto annotate(std::shared_ptr<AstNode> ast) -> std::shared_ptr<AstNode> = 0;
};
} // namespace sp
