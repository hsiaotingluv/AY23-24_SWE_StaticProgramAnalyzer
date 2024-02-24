#include "common/ast/statement_ast.hpp"

/* These nodes follow the specs on
 * https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/abstract-syntax-tree.html
 * It means that variable is not stored as a new name node, but as the content of read node itself
 */
namespace sp {
auto StatementNode::set_statement_number(uint32_t statement_number) -> void {
    this->statement_number = statement_number;
}

[[nodiscard]] auto StatementNode::get_statement_number() const -> int {
    return statement_number.value_or(-1);
}

[[nodiscard]] auto StatementNode::has_statement_number() const -> bool {
    return statement_number.has_value();
}
} // namespace sp
