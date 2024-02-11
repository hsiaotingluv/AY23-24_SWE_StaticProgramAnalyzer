#include "sp/annotator/ast_annotator.hpp"

namespace sp {
auto AstAnnotator::annotate(std::shared_ptr<AstNode> ast) -> std::shared_ptr<AstNode> {
    for (const auto& annotator : annotators) {
        ast = annotator->annotate(ast);
    }
    return ast;
}
} // namespace sp