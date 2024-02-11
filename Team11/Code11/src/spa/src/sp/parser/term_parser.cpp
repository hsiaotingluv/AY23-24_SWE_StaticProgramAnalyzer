#include "sp/parser/term_parser.hpp"

namespace sp {
auto TermParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end) -> std::shared_ptr<AstNode> {
    auto factor_tree = factor_parser.parse(token_start, token_end);
    auto bottom_top_node = this->parseTermPrime(token_start, token_end);

    auto bottom_node = std::get<0>(bottom_top_node);
    auto top_node = std::get<1>(bottom_top_node);

    if (bottom_node->T == NodeType::EmptyString) {
        return factor_tree;
    } else {
        auto casted_node = std::static_pointer_cast<BinaryNode>(bottom_node);
        casted_node->left = factor_tree;
        return top_node;
    }
}

auto TermParser::parseTermPrime(Parser::Iterator& token_start, // NOLINT(*-no-recursion)
                                const Parser::Iterator& token_end)
    -> std::tuple<std::shared_ptr<AstNode>, std::shared_ptr<AstNode>> {
    check_has_token(token_start, token_end);

    auto next_token = peek_next_token(token_start);

    if (next_token.T != TokenType::Div && next_token.T != TokenType::Mod && next_token.T != TokenType::Mul) {
        // empty string production
        return std::make_tuple(std::make_shared<NullNode>(), std::make_shared<NullNode>());
    }

    // Div, Mod, Mul
    get_next_token(token_start);

    std::shared_ptr<BinaryNode> new_partial_bottom;
    if (next_token.T == TokenType::Mul) {
        new_partial_bottom = std::make_shared<BinaryNode>(NodeType::Mul);
    } else if (next_token.T == TokenType::Div) {
        new_partial_bottom = std::make_shared<BinaryNode>(NodeType::Div);
    } else if (next_token.T == TokenType::Mod) {
        new_partial_bottom = std::make_shared<BinaryNode>(NodeType::Mod);
    }

    new_partial_bottom->left = nullptr;
    auto factor_node = factor_parser.parse(token_start, token_end);
    new_partial_bottom->right = factor_node;

    auto bottom_top_node = this->parseTermPrime(token_start, token_end);
    auto bottom_node = std::get<0>(bottom_top_node);
    auto top_node = std::get<1>(bottom_top_node);

    if (bottom_node->T == NodeType::EmptyString) {
        // new top
        return std::make_tuple(new_partial_bottom, new_partial_bottom);
    } else {
        auto casted_node = std::static_pointer_cast<BinaryNode>(bottom_node);
        casted_node->left = new_partial_bottom;
        return std::make_tuple(new_partial_bottom, top_node);
    }
}
} // namespace sp
