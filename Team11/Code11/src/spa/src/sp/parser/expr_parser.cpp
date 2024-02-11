#include "sp/parser/expr_parser.hpp"
#include "sp/parser/ast/binary_node_ast.hpp"
#include "sp/parser/ast/null_ast.hpp"
#include "sp/parser/term_parser.hpp"

namespace sp {
auto ExprParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end) -> std::shared_ptr<AstNode> {
    TermParser term_parser; // circumvent forward declaration due to circular header dependency

    auto term_tree = term_parser.parse(token_start, token_end);
    auto bottom_top_node = this->parseExprPrime(token_start, token_end);

    auto bottom_node = std::get<0>(bottom_top_node);
    auto top_node = std::get<1>(bottom_top_node);

    if (bottom_node->T == NodeType::EmptyString) {
        return term_tree;
    } else {
        auto casted_node = std::dynamic_pointer_cast<BinopNode>(bottom_node);
        casted_node->left = term_tree;
        return top_node;
    }
}

auto ExprParser::parseExprPrime(Parser::Iterator& token_start, // NOLINT(*-no-recursion)
                                const Parser::Iterator& token_end)
    -> std::tuple<std::shared_ptr<AstNode>, std::shared_ptr<AstNode>> {
    TermParser term_parser; // circumvent forward declaration due to circular header dependency

    check_has_token(token_start, token_end);

    auto next_token = peek_next_token(token_start);

    switch (next_token.T) {
    case TokenType::Add:
    case TokenType::Sub: {
        get_next_token(token_start);

        std::shared_ptr<BinopNode> new_partial_bottom;
        if (next_token.T == TokenType::Add) {
            new_partial_bottom = std::make_shared<AddNode>();
        } else if (next_token.T == TokenType::Sub) {
            new_partial_bottom = std::make_shared<SubNode>();
        }

        new_partial_bottom->left = nullptr;
        auto factor_node = term_parser.parse(token_start, token_end);
        new_partial_bottom->right = factor_node;

        auto bottom_top_node = this->parseExprPrime(token_start, token_end);
        auto bottom_node = std::get<0>(bottom_top_node);
        auto top_node = std::get<1>(bottom_top_node);

        if (bottom_node->T == NodeType::EmptyString) {
            // new top
            return std::make_tuple(new_partial_bottom, new_partial_bottom);
        } else {
            auto casted_node = std::dynamic_pointer_cast<BinopNode>(bottom_node);
            casted_node->left = new_partial_bottom;
            return std::make_tuple(new_partial_bottom, top_node);
        }
    }
    default:
        // invalid empty token
        return std::make_tuple(std::make_shared<NullNode>(), std::make_shared<NullNode>());
    }
}
} // namespace sp
