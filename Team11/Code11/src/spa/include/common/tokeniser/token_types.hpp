#pragma once

namespace tokenizer {
enum class TokenType {
    // Binary Operator
    Add,
    Sub,
    Mul,
    Div,
    Mod,

    // Primitive Type
    String,
    Integer,

    // Delimiter
    LParen,
    RParen,
    Semicolon,

    // Junk
    Junk,

    // ============= QPS specifics Token ==============
    // QPS Delimiter
    DQuote,
    Comma,

    // QPS Wildcard Token
    Wildcard,

    // ============= SP specifics Token ===============
    // Keyword is captured as string since keyword names are not reserved
    // SP Delimiter
    LCurly,
    RCurly,

    // SP Relational Token
    LessThan,
    LessThanEqual,
    GreaterThan,
    GreaterThanEqual,
    DoubleEqual,
    NotEqual,

    // SP Assignment, this has similar token to relational
    Assignment,

    // SP Logical Conditional Token
    LNot,
    LAnd,
    LOr,
};
}