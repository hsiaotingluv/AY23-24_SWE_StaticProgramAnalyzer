#include "sp/parser/statement_parser.hpp"
#include "common/ast/ast.hpp"
#include "common/ast/factor_ast.hpp"
#include "common/ast/statement_ast.hpp"
#include "sp/parser/statement_keyword_const.hpp"
#include "sp/parser/statement_list_parser.hpp"

namespace sp {
auto StatementParser::parse(Parser::Iterator& token_start, const Parser::Iterator& token_end)
    -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);

    auto next_token = get_next_token(token_start);
    switch (next_token.T) {
    case TokenType::String: {
        return parseStmtPrime(token_start, token_end, next_token.content);
    }
    default:
        throw ParsingError("Expected variable name or special keyword token, but other token found");
    }
}

auto StatementParser::parseStmtPrime(Parser::Iterator& token_start, const Parser::Iterator& token_end,
                                     const std::string& prev_content) -> std::shared_ptr<AstNode> {
    check_has_token(token_start, token_end);
    auto next_token = get_next_token(token_start);

    // assign: var_name '=' expr ';'
    if (next_token.T == TokenType::Assignment) {
        auto expr_tree = expr_parser.parse(token_start, token_end);
        auto semicolon = get_next_token(token_start);
        if (semicolon.T != TokenType::Semicolon) {
            throw ParsingError("Expecting ; in assignment but found other token");
        }

        auto new_node = std::make_shared<AssignmentNode>(std::make_shared<VarNode>(prev_content), expr_tree);
        return new_node;
    } else {
        if (prev_content == READ) {
            return parseReadStmt(token_start, next_token);
        } else if (prev_content == PRINT) {
            return parsePrintStmt(token_start, next_token);
        } else if (prev_content == CALL) {
            return parseCallStmt(token_start, next_token);
        } else if (prev_content == IF) {
            return parseIfStmt(token_start, token_end, next_token);
        } else if (prev_content == WHILE) {
            return parseWhileStmt(token_start, token_end, next_token);
        }
    }

    throw ParsingError("Expecting to be one of the second token in statement but found other token");
}

// print: 'print' var_name';'
auto StatementParser::parsePrintStmt(Parser::Iterator& token_start, const Token& next_token)
    -> std::shared_ptr<AstNode> {
    if (next_token.T != TokenType::String) {
        throw ParsingError("Expecting varname for print but found other token");
    }

    auto semicolon = get_next_token(token_start);
    if (semicolon.T != TokenType::Semicolon) {
        throw ParsingError("Expecting ; in assignment but found other token");
    }

    return std::make_shared<PrintNode>(next_token.content);
}

// read: 'read' var_name';'
auto StatementParser::parseReadStmt(Parser::Iterator& token_start, const Token& next_token)
    -> std::shared_ptr<AstNode> {
    if (next_token.T != TokenType::String) {
        throw ParsingError("Expecting varname for read but found other token");
    }

    auto semicolon = get_next_token(token_start);
    if (semicolon.T != TokenType::Semicolon) {
        throw ParsingError("Expecting ; in assignment but found other token");
    }

    return std::make_shared<ReadNode>(next_token.content);
}

// call: 'call' proc_name';'
auto StatementParser::parseCallStmt(Parser::Iterator& token_start, const Token& next_token)
    -> std::shared_ptr<AstNode> {
    if (next_token.T != TokenType::String) {
        throw ParsingError("Expecting varname for call but found other token");
    }

    auto semicolon = get_next_token(token_start);
    if (semicolon.T != TokenType::Semicolon) {
        throw ParsingError("Expecting ; in assignment but found other token");
    }

    return std::make_shared<CallNode>(next_token.content);
}

// if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
auto StatementParser::parseIfStmt(Parser::Iterator& token_start, const Parser::Iterator& token_end,
                                  const Token& next_token) -> std::shared_ptr<AstNode> {
    StatementListParser statement_list_parser; // avoid circular dependency
    if (next_token.T != TokenType::LParen) {
        throw ParsingError("Expecting ( for if but found other token");
    }

    auto cond_expr_tree = cond_expr_parser.parse(token_start, token_end);
    auto rparen_token = get_next_token(token_start);
    if (rparen_token.T != TokenType::RParen) {
        throw ParsingError("Expecting ) in if but found other token");
    }

    auto then_string_token = get_next_token(token_start);
    if (then_string_token.T != TokenType::String || then_string_token.content != THEN) {
        throw ParsingError("Expecting then keyword in if but found other token");
    }

    auto lcurly_token = get_next_token(token_start);
    if (lcurly_token.T != TokenType::LCurly) {
        throw ParsingError("Expecting then { keyword in if but found other token");
    }

    auto then_stmt_list_tree = statement_list_parser.parse(token_start, token_end);

    auto rcurly_token = get_next_token(token_start);
    if (rcurly_token.T != TokenType::RCurly) {
        throw ParsingError("Expecting then } keyword in if but found other token");
    }

    auto else_string_token = get_next_token(token_start);
    if (else_string_token.T != TokenType::String || else_string_token.content != ELSE) {
        throw ParsingError("Expecting else keyword in if but found other token");
    }

    lcurly_token = get_next_token(token_start);
    if (lcurly_token.T != TokenType::LCurly) {
        throw ParsingError("Expecting else { keyword in if but found other token");
    }

    auto else_stmt_list_tree = statement_list_parser.parse(token_start, token_end);

    rcurly_token = get_next_token(token_start);
    if (rcurly_token.T != TokenType::RCurly) {
        throw ParsingError("Expecting else } keyword in if but found other token");
    }

    return std::make_shared<IfNode>(cond_expr_tree, then_stmt_list_tree, else_stmt_list_tree);
}

// while: 'while' '(' cond_expr ')' '{' stmtLst '}'
auto StatementParser::parseWhileStmt(Parser::Iterator& token_start, const Parser::Iterator& token_end,
                                     const Token& next_token) -> std::shared_ptr<AstNode> {
    StatementListParser statement_list_parser; // avoid circular dependency
    if (next_token.T != TokenType::LParen) {
        throw ParsingError("Expecting ( for while but found other token");
    }

    auto cond_expr_tree = cond_expr_parser.parse(token_start, token_end);
    auto rparen_token = get_next_token(token_start);
    if (rparen_token.T != TokenType::RParen) {
        throw ParsingError("Expecting ) in while but found other token");
    }

    auto lcurly_token = get_next_token(token_start);
    if (lcurly_token.T != TokenType::LCurly) {
        throw ParsingError("Expecting while { keyword in while but found other token");
    }

    auto stmt_list_tree = statement_list_parser.parse(token_start, token_end);

    auto rcurly_token = get_next_token(token_start);
    if (rcurly_token.T != TokenType::RCurly) {
        throw ParsingError("Expecting while } keyword in while but found other token");
    }

    return std::make_shared<WhileNode>(cond_expr_tree, stmt_list_tree);
}
} // namespace sp